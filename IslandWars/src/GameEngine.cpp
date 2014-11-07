/*
 *  GameEngine.h
 *  IslandWars
 *
 *  (Revision 427)
 *   *  This file is used by Patrick.
 *	Functions used by Patrick:
 *  Functions under line ~140 => From GameEngine(int nPlayers) constructor
 *	Functions over line 1240 => To isEndGame()
 *	 *
 *
 */

#include "GameEngine.h"
#include "MapLoader.h"
#include "pathFinding.h"
#include "WillsGlobals.h"

#include <boost/pointer_cast.hpp>
//#include "Important.h"

#ifndef DEFAULT_MAP_SIZE
#define DEFAULT_MAP_SIZE 1600
#endif

#ifndef ROW_SIZE
#define ROW_SIZE 40
#endif

#ifndef COL_SIZE
#define COL_SIZE 40
#endif

namespace IslandWars
{

	static GameEngine * me;

	GameEngine * GameEngine::get() {
		return me;
	}

	// --------------------------------------------------------
	// GameEngine class constructor
	//
	// Creates nPlayers players.
	//
	// Note that if there is less than two players the map
	// will be wrong.
	// --------------------------------------------------------
	GameEngine::GameEngine(int nPlayers){

		me = this;

		EventListenerPtr listener( this );
		safeAddListener( listener, AttackEvent::sk_EventType );
		safeAddListener( listener, BombRequestEvent::sk_EventType );
		safeAddListener( listener, BombEvent::sk_EventType );
		safeAddListener( listener, BuildRequestEvent::sk_EventType );
		safeAddListener( listener, BuildCancelledEvent::sk_EventType );
		safeAddListener( listener, BuildEvent::sk_EventType );
		safeAddListener( listener, FortifyRequestEvent::sk_EventType );
		safeAddListener( listener, FortifyEvent::sk_EventType );
		safeAddListener( listener, EndTurnEvent::sk_EventType );
		safeAddListener( listener, MoveRequestEvent::sk_EventType );
		safeAddListener( listener, MoveUnitEvent::sk_EventType );
		safeAddListener( listener, ConquerEvent::sk_EventType );
		safeAddListener( listener, ShellRequestEvent::sk_EventType );
		safeAddListener( listener, ShellEvent::sk_EventType );
		safeAddListener( listener, RepairRequestEvent::sk_EventType );
		safeAddListener( listener, RepairEvent::sk_EventType );
		safeAddListener( listener, SaveGameEvent::sk_EventType );

		objectId = 0;
		playerId = 0;
		actualPlayerIndex = 0;

		totalTurnCompleted = 0;
		actualPlayerIndex = 0;

		//create default player
		createPlayer("Default");

		//create nPlayers players
		int n;

		for (n = 0; n < nPlayers; ++n) {
			createPlayer();
		}

		actualPlayer = getSpecificPlayer(0);
	}

	// --------------------------------------------------------
	// GameEngine class destructor (simple)
	// --------------------------------------------------------
	GameEngine::~GameEngine() { }

	//PLAYER

	void GameEngine::createPlayer(){

		//createPlayer
		PlayerPtr pp( new Player(playerId) );

		//add it to list
		playerList.push_back(pp);

		//playerId number is incremented
		++playerId;

		return;
	}

	/*

	 * GENERAL NOTE for code below
	 * COUT and CERR will need to be replaced by
	 * "DeniedEvents" or "ConfirmEvents".
	 * Their presence right now is only for testing...

	 */

	void GameEngine::createPlayer(std::string name){

		//createPlayer
		PlayerPtr pp( new Player(playerId, name) );

		//add it to list
		playerList.push_back(pp);

		//playerId number is incremented
		++playerId;

		return;
	}

	void GameEngine::setPlayerName(int playerId, std::string name){
		PlayerPtr pp = getSpecificPlayer(playerId);
		pp -> setPlayerName( name );
		return;
	}

	PlayerPtr GameEngine::getActualPlayer(){
		return actualPlayer;
	}

	void GameEngine::resetActualPlayer(){
		actualPlayer = getSpecificPlayer(actualPlayerIndex%playerId);
		return;
	}

	const int GameEngine::getGEPlayerId(){
		//GameEngine's playerId = playerCount
		return playerId;
	}

	PlayerPtr GameEngine::getSpecificPlayer(int playerId){

		//invalid playerId will return NULL pointers
		if ( playerId >= playerList.size() ) {
			return PlayerPtr();
		}

		return playerList.at(playerId);
	}


	//HEXAGON & MAP

	// --------------------------------------------------------
	// Initializes game's Hexagon_Map pointer data member
	// --------------------------------------------------------
	void GameEngine::initializeMap(int *map){
		m_HexMap = new Hexagon_Map;
		m_HexMap->initialize(map);
		createStructuresOnMap( map );
	}

	// --------------------------------------------------------
	// Create Structures on Map
	// --------------------------------------------------------
	int GameEngine::createStructuresOnMap( int *map ){

		// Structure distribution.
		int i;
		int nPlayers = getGEPlayerId() - 1;
		int j = 0;

		// Strategy : Remember all starting towers coordinates
		// and distribute them back to available players from
		// Player #0.
		int startingTowers[MAX_PLAYERS];

		for (i = 0; i < ROW_SIZE*COL_SIZE; ++i) {

			ObjectParams * params;

			// all Structures go first to the default player, Player #0.
			switch (map[i]) {
				case 10:
					params = new ObjectParams;
					params->playerId = 0;
					params->objectId =

					createStructure(0, i/ROW_SIZE, i%COL_SIZE, TOWER);
					break;
				case 11:
					createStructure(0, i/ROW_SIZE, i%COL_SIZE, PORT_CITY);
					break;
				case 12:
					createStructure(0, i/ROW_SIZE, i%COL_SIZE, LAND_CITY);
					break;
				case 13:

					if (j < MAX_PLAYERS) {

						startingTowers[j] = createStructure(0, i/ROW_SIZE, i%COL_SIZE, TOWER);
						++j;
					}
					break;

				default:
					break;

			}

		}

		int k;

		for (k = 1; k <= nPlayers; ++k) {

			if (k % 2 == 0) {

				acquireStructure(0, k, startingTowers[MAX_PLAYERS-(k-1)]);

			}else {

				acquireStructure(0, k, startingTowers[k-1]);

			}

		}

		return 0;

	}

	// --------------------------------------------------------
	// Get Hexagon at coordinates specified in Hexagon_Map
	// --------------------------------------------------------
	HexagonPtr GameEngine::getHexagon(int x, int y) {

		if ( x > MAX_WIDTH-1 || y > MAX_HEIGHT-1 || x < 0 || y < 0 ) {
			return HexagonPtr();
		}

		return m_HexMap->getHexagon( x, y );
	}

	// --------------------------------------------------------
	// Get Hexagon at coordinates specified in Hexagon_Map
	// --------------------------------------------------------
	HexagonPtr GameEngine::getHexagon(Vector2 & position){

		//printf("Get hexagon !\n");

		if ( position.x > MAX_WIDTH-1 || position.y > MAX_HEIGHT-1 || position.x < 0 || position.y < 0 ) {
			//printf("Out of Bounds\n");
			return HexagonPtr();
		}

		return m_HexMap->getHexagon( position.x, position.y );
	}

    // --------------------------------------------------------
	// Applies a boolean function to Vector2s in a given radius.
	// Used to bomb and to detect submarines for example.
	// --------------------------------------------------------
	bool GameEngine::massEffect( HexagonPtr epicenter, bool (GameEngine::*func)( Vector2 & v ), int radius ){

        bool result;
        Vector2 cV(epicenter->getX(), epicenter->getY() );
		result = (this->*func)( cV );
        if(result)
            return result;

        int i, j,k;
        Vector2 spinA[6], spinB[6], * spin;
        Vector2 v0(0, -1);

		spinA[0] = Vector2(1, 1);
		spinA[1] = Vector2(0, 1);
		spinA[2] = Vector2(-1, 0);
		spinA[3] = Vector2(-1, 0);
		spinA[4] = Vector2(0, -1);
		spinA[5] = Vector2(1, -1);

		spinB[0] = Vector2(1, 0);
		spinB[1] = Vector2(0, 1);
		spinB[2] = Vector2(-1, 1);
		spinB[3] = Vector2(-1, -1);
		spinB[4] = Vector2(0, -1);
		spinB[5] = Vector2(1, 0);

	    spin = (cV.x % 2) ? spinB : spinA;

        for(i = 1; i <= radius; i++){
            cV = cV + v0;
            for(j = 0; j < 6; j++){
                for(k = 1; k<= i;k++){
					if (i > 1) {
						if ( !(i % 2) && (j == 2 || j == 5)) {
							spin = (spin == spinA) ? spinB : spinA;
						}
					}
                    cV = cV + spin[j];
                    result = (this->*func)( cV );
                    if( result )
                        return result;
					if (i > 1) {
						if ( (i % 2) && (j == 2 || j == 5)) {
							spin = (spin == spinA) ? spinB : spinA;
						}

						if (j == 0 || j == 3) {
							spin = (spin == spinA) ? spinB : spinA;
						}
					}
                }
				if ((j == 0 || j == 2 || j == 3 || j == 5) && i > 1 && (i % 2))
					spin = (spin == spinA) ? spinB : spinA;
            }
        }
        return result;
	}


	//STRUCTURE & UNIT

	// --------------------------------------------------------
	// Create structure for a specific player
	//
	// Used by the map initialization function.
	//
	// Note that the integer returned is the id of the
	// structure in order to conserve it in the distributing
	// technique of the map initialization function.
	//
	// --------------------------------------------------------
	int GameEngine::createStructure(int playerId, int xCoords, int yCoords, ObjectType type){

		//get player pointer
		PlayerPtr pp = getSpecificPlayer(playerId);

		if (pp.get() == NULL) {
			std::cerr << "Trying to add a structure to an uncreated player !";
			return -1;
		}

		//Create parameters to pass to factory
		ObjectParams* op = new ObjectParams();

		op->playerId = playerId;
		op->objectId = objectId;
		op->x_coord = xCoords;
		op->y_coord = yCoords;
		op->type = type;

		CreateObjectEvent * createEvent = new CreateObjectEvent( op );
		IEventDataPtr cePtr( createEvent );
		safeQueueEvent(cePtr);

		//printf("Added event?\n");

		//Call GameEngine's factory to work
		StructurePtr sp( ofact.createStructure(op) );

		//add to player's list
		pp->addStructure(sp);

		if (getHexagon(xCoords,yCoords).get() != 0) {
			getHexagon(xCoords,yCoords) -> addObject( sp );
		}

		//increment objectId
		++objectId;

		return (sp -> getId());
	}


	//Assisting startTurnUpdate if any unit needs to be created
	UnitPtr GameEngine::generateUnit(int pId, ProductionRequestPtr & prp, int x, int y){
		//Parameters to be passed to factory
		ObjectParams * op = new ObjectParams;
		op->playerId = pId;
		op->objectId = prp->getUnitId();
		op->x_coord = x;
		op->y_coord = y;
		op->type = prp->getObjectType();

		//uses GameEngine's factory
		UnitPtr unitPtr = ofact.createUnit(op);

		delete op;

		return unitPtr;
	}

	UnitPtr GameEngine::getUnitPointer(int unitId, Vector2 position){
		HexagonPtr hex = getHexagon( position );

		if (hex.get() == NULL) {
			return UnitPtr();
		}

		UnitPtr unitPtr = hex->getUnitById( unitId );

		if ( unitPtr.get() == NULL ) {
			int transporterId;
			transporterId = isUnitTypeHere( hex, TRANSPORTER );

			if ( transporterId > -1 ) {
				// Look in transporter's list (only one possible transporter transporting units, on one tile)
				unitPtr = actualPlayer -> getTransportedUnit( transporterId, unitId );
			}
		}


		return unitPtr;
	}

	//BUILD

	// --------------------------------------------------------
	// Request Build
	//
	// Triggered by BuildRequestEvent.
	// Generates BuildDeniedEvent and BuildEvent.
	// --------------------------------------------------------
	void GameEngine::requestBuild( int structureId, Vector2 v, ObjectType ot ){

		selectTile(v.x, v.y);

		if (selectedTile.get() == NULL) {
			// Build Request Failed - Could not get the right tile.
			BuildDeniedEvent * buildDeniedEvent = new BuildDeniedEvent( structureId, 4);
			IEventDataPtr buildDeniedPtr( buildDeniedEvent );
			safeQueueEvent( buildDeniedPtr );
			return;
		}

		selectStructure();

		if (lastSelectedStructure.get() == NULL) {
			// Build Request Failed - Could not get the right structure on this tile.
			BuildDeniedEvent * buildDeniedEvent = new BuildDeniedEvent( structureId, 5);
			IEventDataPtr buildDeniedPtr( buildDeniedEvent );
			safeQueueEvent( buildDeniedPtr );
			return;
		}

		int buildCase;
		buildCase = verifyBuild( ot );

		//printf("\nTurn: %d, Player: %d, Build Case: %d\n", actualPlayer->getTurnNumber(), actualPlayer->getPlayerId(), buildCase);

		switch (buildCase) {
			case 4: {
				// Build Request Failed - Flag Capacity Reached.
				BuildDeniedEvent * buildDeniedEvent = new BuildDeniedEvent( structureId, 4);
				IEventDataPtr buildDeniedPtr( buildDeniedEvent );
				safeQueueEvent( buildDeniedPtr );
				break;
			}
			case 3: {
				// Build Request Failed - Flag Capacity Reached.
				BuildDeniedEvent * buildDeniedEvent = new BuildDeniedEvent( structureId, 3);
				IEventDataPtr buildDeniedPtr( buildDeniedEvent );
				safeQueueEvent( buildDeniedPtr );
				break;
			}
			case 2: {
				// Build Request Failed - Wrong type.
				BuildDeniedEvent * buildDeniedEvent = new BuildDeniedEvent( structureId, 2);
				IEventDataPtr buildDeniedPtr( buildDeniedEvent );
				safeQueueEvent( buildDeniedPtr );
				break;
			}
			case 1: {
				// Build Request Failed - Production Full.
				BuildDeniedEvent * buildDeniedEvent = new BuildDeniedEvent( structureId, 1);
				IEventDataPtr buildDeniedPtr( buildDeniedEvent );
				safeQueueEvent( buildDeniedPtr );
				break;
			}
			case 0: {
				// Build Request Succeeded.
				BuildEvent * buildEvent = new BuildEvent(structureId, v, objectId, ot);
				IEventDataPtr buildPtr( buildEvent );
				safeQueueEvent( buildPtr );
				break;
			}
			default:
				break;
		}

		return;
	}

	// Verification

	// --------------------------------------------------------
	// Verify Build
	//
	// Can lastSelectedStructure start building a unit of this
	// type now ?
	//
	// Return values :
	// 4 : Not current Player !
	// 3 : No more than one flag per player !
	// 0 : Successful !
	// 1 : Production is full for this turn !
	// 2 : Type not supported by this type of structure !
	// --------------------------------------------------------
	int GameEngine::verifyBuild( ObjectType type ){

		if ( actualPlayer -> getPlayerId() != playerID ){
			// Not current Player - Build Request Denied
			return 4;
		}

		if (type == FLAG && ( actualPlayer -> getNFlag() ) > 0) {
			//Special Case #3 : No more than one flag per player !
			return 3;
		}

		return ( lastSelectedStructure -> verifyBuild( type, (actualPlayer->getTurnNumber()) ) );
	}

	// Execution

	// --------------------------------------------------------
	// Build
	//
	// Triggered by BuildEvent
	// --------------------------------------------------------
	void GameEngine::build( int structureId, Vector2 v, int unitId, ObjectType ot ){

		selectTile(v.x, v.y);

		if (selectedTile.get() == NULL) {
			
			//std::cout << "\n\nThe tile that was selected is pointing to NULL\n\n\n";
			
			// Build Request Failed - Could not get the right tile.
			BuildDeniedEvent * buildDeniedEvent = new BuildDeniedEvent( structureId, 4);
			IEventDataPtr buildDeniedPtr( buildDeniedEvent );
			safeQueueEvent( buildDeniedPtr );
			return;
		}

		selectStructure();
		
		if (lastSelectedStructure.get() == NULL) {
			
			//std::cout << "\n\nThe structure that was selected is pointing to NULL\n\n\n";
			
			// Build Request Failed - Could not get the right structure on this tile.
			BuildDeniedEvent * buildDeniedEvent = new BuildDeniedEvent( structureId, 5);
			IEventDataPtr buildDeniedPtr( buildDeniedEvent );
			safeQueueEvent( buildDeniedPtr );
			return;
		}

		handleProductionRequest(ot, unitId);

		return;
	}


	// --------------------------------------------------------
	// Handle production request at specific structure
	// --------------------------------------------------------
	void GameEngine::handleProductionRequest(ObjectType type, int unitId){

		lastSelectedStructure -> handleProductionRequest(unitId, type, (actualPlayer->getTurnNumber()) );

		return;

	}

	// --------------------------------------------------------
	// Build
	//
	// Triggered by CancelBuildRequestEvent
	// --------------------------------------------------------
	void GameEngine::cancelBuild( int structureId, Vector2 v, int unitId ){

		selectTile(v.x, v.y);

		if (selectedTile.get() == NULL) {
			// Cancel Build Request Failed - Could not get the right tile.
			return;
		}

		selectStructure();

		if (lastSelectedStructure.get() == NULL) {
			// Cancel Build Request Failed - Could not get the right structure on this tile.
			return;
		}

		cancelProductionRequest(unitId);

		return;
	}

	// --------------------------------------------------------
	// Cancel production request from structure
	// --------------------------------------------------------
	int GameEngine::cancelProductionRequest(int unitId){

		return (lastSelectedStructure->cancelProductionRequest(unitId));
	}

    // --------------------------------------------------------
	// Checks if there is a Structure at the given Vector2
	// and if that structure belongs to a different player
	// Intended to be called for BombRequest handling
	// --------------------------------------------------------
	bool GameEngine::isStructure( Vector2 & v ){
		HexagonPtr hex = getHexagon(v);
		if( hex->getStructure() && ( ( hex->getStructure() )->getPlayer() != actualPlayer->getPlayerId() ) ){
			return true;
		}
		return false;
	}


	//SPECIAL ABILITIES

	bool GameEngine::canAct( UnitPtr unitToAct ){

		if (unitToAct.get() == NULL) {
			return false;
		}

		bool canActNow = !(unitToAct->getActed());

	    return canActNow;
	}

	// --------------------------------------------------------
	// isSonar
	//
	// Check if there is an Unit with sonar abilities at coords
	// given, that is not of current player id.
	// --------------------------------------------------------
	bool GameEngine::isSonar( Vector2& v ){

		int result;
		result = false;

		if (v.x < MAX_WIDTH || v.y < MAX_WIDTH) {
			HexagonPtr hp = getHexagon(v.x, v.y);

			StructurePtr sp = hp -> getStructure();

			if (sp.get() != NULL) {
				if ( (sp -> getPlayer()) == (actualPlayer -> getPlayerId()) ) {
					int i = 0;
					UnitPtr garrisonedUnit = sp -> getGarrisonByPosition(i);

					result = false; // will not change if no unit with ABILITY_SONAR is found.

					while ( garrisonedUnit.get() != NULL ){

						if ( ( garrisonedUnit -> getAbilities() ) && ABILITY_SONAR ){
							// The unit is sonarized from this location !
							result = true;
							break;

						}

						i++;
						garrisonedUnit = sp -> getGarrisonByPosition(i);

					}

				}else {
					//Not your structure, actualPlayer can't have submarine over there.
					result = false;

				}

			}else {
				//No structure on this tile.
				int i = 0;
				UnitPtr unitOnTile = hp -> getUnitByPosition(i);

				result = false; // will not change if no unit with ABILITY_SONAR is found.

				while ( unitOnTile.get() != NULL ){

					//all units on a tile are checked independently of each other
					if ( ( unitOnTile -> getPlayer() ) == ( actualPlayer -> getPlayerId() ) ) {
						if ( ( unitOnTile -> getAbilities() ) && ABILITY_SONAR ){
							// The unit is sonarized for this location !
							result = true;
							break;

						}

					}

					i++;
					unitOnTile = hp -> getUnitByPosition(i);

				}
			}


		}else {
			//Out of bounds
			result = false;

		}

		return result;

	}

	bool GameEngine::canAttack(UnitPtr defender, UnitPtr attacker) {

	    if( attacker->getAbilities() & ABILITY_SEA_ATTACK )
        {
            if( defender->getUnitClass() != UC_SEA)
            {
                return false;
            }
        }

        return true;
	}

	//Determines if a unit should be deleted and deletes it if necessary
	//based on Hitpoints
	void GameEngine::lifeJudgement(UnitPtr unitPtr){

		if(unitPtr->getRemainingHitpoints() == 0)
		{
			destroyUnit(unitPtr);
		}
	}

	void GameEngine::battleUnit(UnitPtr defender, UnitPtr attacker) {

	    //Checks if trying to battle with NULL pointers
	    if( defender.get() == NULL || attacker.get() == NULL )
	    {
	        return;
	    }

		defender->setRemainingHitpoints( defender->getRemainingHitpoints() - attacker->getAttack() );
		
		if( canAttack( attacker, defender ) )
		{
			//std::cout << "Submarine Case attack." << std::endl;
			attacker->setRemainingHitpoints( attacker->getRemainingHitpoints() - defender->getAttack() );
		}
		
		lifeJudgement(attacker);
		lifeJudgement(defender);
		
		attacker->setActed( true );
		attacker->setRemainingMoves( 0 );

	}

	//This initiates a battle between a unit and a sector
	//AttackRequestEvent goes through here
	void GameEngine::attackSector(HexagonPtr sector, UnitPtr attacker ) {

		if( attacker.get() == NULL )
		{
			return;
		}

		//Checks if indeed the unit can attack this turn
		if( attacker->getActed() )
		{

			Vector2 position( attacker->getX(), attacker->getY() );
			AttackDeniedEvent * deniedAttackUnit = new AttackDeniedEvent( attacker->getId(), position );
			IEventDataPtr denyAttackPtr( deniedAttackUnit );
			safeQueueEvent( denyAttackPtr );
			return;
		}

		int i = 0;
        UnitPtr defender;
		StructurePtr attackedStructure;

		if( sector->getStructure() )
		{
			attackedStructure = sector->getStructure();
			do{
				defender = attackedStructure->getGarrisonByPosition(i);
				i++;
			}while( defender && !GameEngine::canAttack( defender, attacker ) );

			if( defender.get() == NULL ){
				if( attacker->getUnitClass() != UC_LAND ){
					Vector2 position( attacker->getX(), attacker->getY() );
					AttackDeniedEvent * deniedAttackUnit = new AttackDeniedEvent( attacker->getId(), position );
					IEventDataPtr denyAttackPtr( deniedAttackUnit );
					safeQueueEvent( denyAttackPtr );
					return;
				}else{
					Vector2 positionAttacker( attacker->getX(), attacker->getY() );
					Vector2 positionStructure( sector->getStructure()->getX(), sector->getStructure()->getY() );
					ConquerEvent * conquerEvent = new ConquerEvent( attacker->getId(), positionAttacker, sector->getStructure()->getId(), positionStructure );
					IEventDataPtr conquerPtr( conquerEvent );
					safeQueueEvent( conquerPtr );
					return;
				}
			}
			attackedStructure->setUnderAttack( true );
			Vector2 positionAttacker( attacker->getX(), attacker->getY() );
			Vector2 positionDefender( defender->getX(), defender->getY() );
			AttackEvent * attackUnit = new AttackEvent( attacker->getId(), positionAttacker, defender->getId(), positionDefender );
			IEventDataPtr attackUnitPtr( attackUnit );
			safeQueueEvent( attackUnitPtr );
			return;
		}

		else
		{
			i = 0;

			do{
				defender = sector->getUnitByPosition(i);
				i++;
			}while( defender && !GameEngine::canAttack(defender, attacker) );

			if( defender.get() == NULL )
			{
				Vector2 position( attacker->getX(), attacker->getY() );
				AttackDeniedEvent * deniedAttackUnit = new AttackDeniedEvent( attacker->getId(), position );
				IEventDataPtr denyAttackPtr( deniedAttackUnit );
				safeQueueEvent( denyAttackPtr );
				return;
			}
		}
		//This is an attack against another unit
		Vector2 positionAttacker( attacker->getX(), attacker->getY() );
		Vector2 positionDefender( defender->getX(), defender->getY() );
		AttackEvent * attackUnit = new AttackEvent( attacker->getId(), positionAttacker, defender->getId(), positionDefender );
		IEventDataPtr attackUnitPtr( attackUnit );
		safeQueueEvent( attackUnitPtr );
		return;
	}

	void GameEngine::shell(UnitPtr shelley, UnitPtr sheller){

        //Checks if NULL pointers
        if(shelley.get() == NULL || sheller.get() == NULL)
        {
            return;
        }

        if( sheller->getAbilities() & ABILITY_SHELL )
        {
            shelley->setRemainingHitpoints( shelley->getRemainingHitpoints() - 1 );
            lifeJudgement(shelley);
            sheller->setActed( true );
			sheller->setRemainingMoves( 0 );
        }

	}

	bool GameEngine::bomb( Vector2 & groundZero ){
        HexagonPtr gnd0 = getHexagon( groundZero );
        std::list<UnitPtr> listToDestroy = gnd0->getListOfUnits();
        std::list<UnitPtr>::iterator it;

        for( it = listToDestroy.begin(); it != listToDestroy.end(); it++ )
			GameEngine::destroyUnit( (*it) );

        StructurePtr structureToDestroy = gnd0->getStructure();
        if( structureToDestroy.get() != NULL ){
            listToDestroy = structureToDestroy->clearGarrison();
            for( it = listToDestroy.begin(); it != listToDestroy.end(); it++ )
                GameEngine::destroyUnit( (*it) );
        }

        return false;
	}

	void GameEngine::repair(UnitPtr unitToRepair){

        if(unitToRepair.get() == NULL)
        {
            return;
        }

        unitToRepair->setRemainingHitpoints( unitToRepair->getRemainingHitpoints() + 1 );
		unitToRepair->setRepaired( true );
        unitToRepair->setRemainingMoves( 0 );
    }


    void GameEngine::refuel(AirUnitPtr unitToRefuel){

        if(unitToRefuel.get() == NULL)
        {
            return;
        }

        unitToRefuel->setFuel( unitToRefuel->getMaxFuel() );

    }

	void GameEngine::defuel(AirUnitPtr unitToRefuel){

        if(unitToRefuel.get() == NULL)
        {
            return;
        }

        unitToRefuel->setFuel( unitToRefuel->getFuel() - 1 );
        if( unitToRefuel->getFuel() == 0 ){
            PlayerPtr p1 = getSpecificPlayer( unitToRefuel ->getPlayer() );
            p1 -> removePath(unitToRefuel->getId() );
            destroyUnit(unitToRefuel);
        }

    }

	void GameEngine::fortify(UnitPtr unitToFortify, bool fortify ){

		if (unitToFortify.get() == NULL) {
			return;
		}

	    unitToFortify->setFortified( fortify );
	    lifeJudgement( unitToFortify );
	}

	void GameEngine::conquer( UnitPtr unitConquering, StructurePtr structureToConquer ){

		if( 0 == structureToConquer->garrisonSize() ){

			switch( structureToConquer->getObjectType() ){

				case TOWER:
					if( unitConquering->getAbilities() & ABILITY_COLONIZE ){
						acquireStructure( structureToConquer->getPlayer(), unitConquering->getPlayer(), structureToConquer->getId() );
						destroyUnit( unitConquering );
					}
					break;
				default:
					acquireStructure( structureToConquer->getPlayer(), unitConquering->getPlayer(), structureToConquer->getId() );
					destroyUnit( unitConquering );
			}
		}
	}

	//used by startTurnUpdates to reset "movesLeft", "acted" and "repaired" data members
	void GameEngine::resetUnit( UnitPtr up ){

        
		int m = up -> getMaxMoves();
		up -> setRemainingMoves( m );
        
		up -> setActed( false );
		up-> setRepaired( false );

		return;
	}

	void GameEngine::updateFuel( AirUnitPtr up, HexagonPtr hp){

		StructurePtr sp = hp -> getStructure();

		//structure ?
		if ( sp.get() != NULL ) {

			if ( (sp -> getPlayer() ) == (up -> getPlayer() ) ) {

				//plane has an allied structure around...
				refuel(up);

			}else {

				//plane is over some enemy structure I guess
				defuel(up);
			}


		}else {
			//carrier ?
			bool carrierHere = false;

			std::list<UnitPtr>::iterator iter;
			std::list<UnitPtr> h_unitList = hp -> getListOfUnits();

			UnitPtr possibleCarrier;

			for (iter = h_unitList.begin(); iter != h_unitList.end(); ++iter) {

				possibleCarrier = (*iter);

				if ( possibleCarrier.get() != NULL ) {
					if ( possibleCarrier -> getObjectType() == CARRIER) {
						carrierHere = true;
						break;
					}
				}

			}

			if (carrierHere) {
				refuel(up);
			}else {
				defuel(up);
			}
		}

		return;

	}


	//MANAGEMENT

	//Destroys a Unit nicely and tidely wherever it is
	//From Boost docs : "The object pointed to is guaranteed to be deleted
	//when the last shared_ptr pointing to it is destroyed or reset."
	void GameEngine::destroyUnit(UnitPtr unitToDestroy){

		//First remove it from the Hexagon it is on.
		HexagonPtr sector = getHexagon( unitToDestroy->getX(), unitToDestroy->getY() );
		sector->remove( unitToDestroy );

		//Then Remove from Player
		int uId = unitToDestroy->getId();
		int pId = unitToDestroy->getPlayer();

		PlayerPtr pPtr = getSpecificPlayer( pId);
		pPtr->deleteUnit( uId );

		//Send Event saying that the unit is destroyed
		UnitDestroyedEvent * destroyUnit = new UnitDestroyedEvent( uId );
		IEventDataPtr destroyPtr( destroyUnit );
		safeQueueEvent( destroyPtr );

		//Then delete passed pointer
		//The pointer cleans up after himself on his own

	}

	// --------------------------------------------------------
	// Acquire/Exchange Structure between Players
	//
	// Intended to be called upon results of attacks b/w
	// units and structures.
	//
	// Generates acquireStructureEvents.
	// --------------------------------------------------------
	void GameEngine::acquireStructure( PlayerPtr frompp, PlayerPtr topp, StructurePtr sp ){

		int toPlayerId = topp -> getPlayerId();
		int fromPlayerId = frompp -> getPlayerId();
		int structureId = sp -> getId();

		//change structure's information
		sp->setPlayer( toPlayerId );

		//addStructure to winner's list
		topp->addStructure(sp);

		//reset production requests
		sp->clearProduction();

		//reset garrison if any unit was not cleaned
		std::list<UnitPtr> upl = sp->clearGarrison();


		if ( !upl.empty() ) {
			std::list<UnitPtr>::iterator iter;
			for (iter = upl.begin(); iter != upl.end(); ++iter){
				UnitPtr up = (*iter);

				//destroy unit from player's list
				frompp -> deleteUnit( up -> getId() );

				// Note that we don't need to delete it
				// from the tile since it only exists
				// in the garrison.
			}
		}

		//deleteStructure from loser's list
		int i = frompp->deleteStructure( structureId );

		if (!i) {
			std::cerr << "Couldn't find the structure anymore... (BIZARRE)\n";
			return;
		}


		AcquiredStructureEvent * acquiredStructure = new AcquiredStructureEvent( structureId, toPlayerId );
		IEventDataPtr acquiredStructurePtr( acquiredStructure );
		safeQueueEvent(acquiredStructurePtr);

		if (fromPlayerId != 0) {
			//Check if loser is GameOver => no more towers
			isGameOver( fromPlayerId );
		}

		return;
	}


	// --------------------------------------------------------
	// Acquire/Exchange Structure between Players
	// (slower, but callable by a request event directly...)
	//
	// Intended to be called upon results of attacks b/w
	// units and structures.
	//
	// Generates acquireStructureEvents.
	// --------------------------------------------------------
	void GameEngine::acquireStructure( int fromPlayer, int toPlayer, int structureId ){

		PlayerPtr frompp = getSpecificPlayer(fromPlayer);

		if (frompp.get() == NULL) {
			std::cerr << "Player #" << fromPlayer << " does not exist.\n";
			return;
		}

		PlayerPtr topp = getSpecificPlayer(toPlayer);
		if (topp.get() == NULL) {
			std::cerr << "Player #" << toPlayer << " does not exist.\n";
			return;
		}

		//get structure pointer from player's list
		StructurePtr sp = frompp -> getStructure(structureId);

		if (sp.get() == NULL) {
			std::cerr << "The structure you requested does not exist in Player #" << fromPlayer << "'s list.\n";
			return;
		}


		//change structure's information
		sp->setPlayer( toPlayer );

		//addStructure to winner's list
		topp->addStructure(sp);


		//reset production requests
		sp->clearProduction();

		//reset garrison if any unit was not cleaned
		std::list<UnitPtr> upl = sp->clearGarrison();


		if ( !upl.empty() ) {
			std::list<UnitPtr>::iterator iter;
			for (iter = upl.begin(); iter != upl.end(); ++iter){
				UnitPtr up = (*iter);

				//destroy unit from player's list
				frompp -> deleteUnit( up -> getId() );

				// Note that we don't need to delete it
				// from the tile since it only exists
				// in the garrison.
			}
		}

		//deleteStructure from loser's list
		int i = frompp->deleteStructure( structureId );

		if (!i) {
			std::cerr << "Couldn't find the structure anymore... (BIZARRE)\n";
			return;
		}

		AcquiredStructureEvent * acquiredStructure = new AcquiredStructureEvent( structureId, toPlayer );
		IEventDataPtr acquiredStructurePtr( acquiredStructure );
		safeQueueEvent(acquiredStructurePtr);


		if (fromPlayer != 0) {
			//Check if loser is GameOver => no more towers
			isGameOver( fromPlayer );
		}

		return;
	}

	//SELECTION
	// --------------------------------------------------------
	// Update selectedTile data member
	//
	// To simplify logical implementation
	// --------------------------------------------------------
	void GameEngine::selectTile(int x, int y){

		lastSelectedTile = selectedTile;
		selectedTile = getHexagon(x,y);
		return;
	}

	// --------------------------------------------------------
	// Update lastSelectedStructure data member
	//
	// To simplify logical implementation
	// --------------------------------------------------------
	void GameEngine::selectStructure(){

		lastSelectedStructure = selectedTile -> getStructure();
		return;
	}


	// --------------------------------------------------------
	// Is a Unit with this ObjectType in Hexagon's list ?
	//
	// Returns unitId for Unit found or -1.
	// --------------------------------------------------------
	int GameEngine::isUnitTypeHere ( HexagonPtr hp, ObjectType typeSought ){

		int unitIdFound;
		unitIdFound = -1;

		int i;
		i = 0;

		UnitPtr potentialUnit;
		potentialUnit = hp -> getUnitByPosition(i);

		while ( potentialUnit.get() != NULL ) {

			if ( ( potentialUnit -> getObjectType() ) == typeSought ) {

				unitIdFound = potentialUnit -> getId();
				break;

			}

			i++;
			potentialUnit = hp -> getUnitByPosition(i);

		}

		return unitIdFound;
	}

	// --------------------------------------------------------
	// Update lastSelectedUnit data member.
	//
	// To simplify logical implementation.
	// Verifies if not found on Hexagon, if it could be in
	// any transport.
	// --------------------------------------------------------
	void GameEngine::selectUnit(int uId){

		lastSelectedUnit = selectedTile -> getUnitById(uId);

		if ( lastSelectedUnit.get() == NULL && selectedTile.get() != NULL ) {
			int transporterId;
			transporterId = isUnitTypeHere( selectedTile, TRANSPORTER );

			if ( transporterId > -1 ) {
				// Look in transporter's list (only one possible transporter transporting units, on one tile)
				lastSelectedUnit = actualPlayer -> getTransportedUnit( transporterId, uId );
			}
		}

		return;
	}

	void GameEngine::selectUnit(int playerId, int uId){

		lastSelectedUnit = selectedTile -> getUnitById(uId);

		if ( lastSelectedUnit.get() == NULL && selectedTile.get() != NULL ) {
			int transporterId;
			transporterId = isUnitTypeHere( selectedTile, TRANSPORTER );

			if ( transporterId > -1 ) {

				PlayerPtr p = getSpecificPlayer(playerId);

				// Look in transporter's list (only one possible transporter transporting units, on one tile)
				lastSelectedUnit = p -> getTransportedUnit( transporterId, uId );
			}
		}

		return;
	}

	//MOVE


	// --------------------------------------------------------
	// Request Move
	//
	// Generates Path and move Unit as far as possible on it.
	// --------------------------------------------------------
	void GameEngine::requestMove( int unitId, Vector2 from, Vector2 to){

		if (actualPlayer -> getPlayerId() != playerID) {
			//Move Request Denied - Player does not have authority on this turn.
			MoveDeniedEvent * moveDeniedEvent = new MoveDeniedEvent( unitId, from );
			IEventDataPtr moveDeniedEventPtr( moveDeniedEvent );
			safeQueueEvent( moveDeniedEventPtr );
			return;
		}


		// Check for moving over same tile
		if ( from.x == to.x && from.y == to.y ) {
			//Move Request Denied - Not a valid move.
			MoveDeniedEvent * moveDeniedEvent = new MoveDeniedEvent( unitId, from );
			IEventDataPtr moveDeniedEventPtr( moveDeniedEvent );
			safeQueueEvent( moveDeniedEventPtr );
			return;
		}

        //Retrieves Unit information for PathFinding
		HexagonPtr hp = getHexagon(from);

		if  ( hp.get() == NULL ){

		    //std::cout << "Move Request Failed. - Could not find the Hexagon at coordinates : (" << from.x << "," << from.y << ").\n";
		    MoveDeniedEvent * moveDeniedEvent = new MoveDeniedEvent( unitId, from );
			IEventDataPtr moveDeniedEventPtr( moveDeniedEvent );
			safeQueueEvent( moveDeniedEventPtr );
		    return;
		}

		UnitPtr up = getUnitPointer(unitId, from );

		if  ( up.get() == NULL ){

		    //std::cout << "Move Request Failed. - Could not find Unit #" << unitId << " at ("  << from.x << "," << from.y << ").\n";
		    MoveDeniedEvent * moveDeniedEvent = new MoveDeniedEvent( unitId, from );
			IEventDataPtr moveDeniedEventPtr( moveDeniedEvent );
			safeQueueEvent( moveDeniedEventPtr );
		    return;
		}
		
		if(up->isFortified()){
			//std::cout << "Move Request Failed. - Fortified Unit #" << unitId << " at ("  << from.x << "," << from.y << ").\n";
		    MoveDeniedEvent * moveDeniedEvent = new MoveDeniedEvent( unitId, from );
			IEventDataPtr moveDeniedEventPtr( moveDeniedEvent );
			safeQueueEvent( moveDeniedEventPtr );
		    return;
		}
			

		UnitClass uc = up -> getUnitClass();

		//Generate Path
		PathFinding pf;
		std::list<Vector2> pathGen;
		pathGen = pf.findPath( from, to, uc );

		if (pathGen.empty()) {

			//printf("Move Request Failed. - Could not get a non-empty path.");

			MoveDeniedEvent * moveDeniedEvent = new MoveDeniedEvent( unitId, from );
			IEventDataPtr moveDeniedEventPtr( moveDeniedEvent );
			safeQueueEvent( moveDeniedEventPtr );
			return;

		}

        //Saves Path generated in Player
		actualPlayer -> setPath( unitId , pathGen );

		//Trigger Move
		Vector2 curPos, nextPos;

        //printf( "Move Request - Path generated : ");

		curPos = actualPlayer->front(unitId);
		actualPlayer->pop(unitId);

		nextPos = actualPlayer->front(unitId);

		//std::cout<<"Unitary move from ("  << curPos.x << "," << curPos.y << ") to ("  << nextPos.x << "," << nextPos.y << ").\n";

		if (actualPlayer->pathSize(unitId) == 1) {
			unitaryMove(actualPlayer -> getPlayerId(), unitId, curPos, nextPos, 1);
		}else {
			unitaryMove(actualPlayer -> getPlayerId(), unitId, curPos, nextPos, 0);
		}

		return;
	}

	// --------------------------------------------------------
	// Unitary Move
	//
	//
	// --------------------------------------------------------
	void GameEngine::unitaryMove( int playerId, int unitId, Vector2 from, Vector2 to, bool isDestination ){

		PlayerPtr playerPtr = getSpecificPlayer(playerId);

        //std::cout << " --- Unitary Move --- \n";
		selectTile(from.x, from.y);

		if (selectedTile.get() == NULL) {
			//Move request failed - Could not find source tile.
			return;
		}

		selectUnit(playerId, unitId);

		if ( lastSelectedUnit.get() == NULL ) {
			//Move request failed - Could not find unit on source tile
			return;
		}

		selectTile(to.x, to.y);

		if (selectedTile.get() == NULL) {
			//Move request failed - Could not find destination tile.
			return;
		}

       // std::cout << "Is it movable ? ";

		switch ( verifyMovability(isDestination) ) {
			case 0: {
				// Regular move.
				// Move Event is created.
				MoveUnitEvent * moveUnitEvent = new MoveUnitEvent( playerId, unitId, from, to, 0 );
				IEventDataPtr moveUnitPtr( moveUnitEvent );
				safeQueueEvent( moveUnitPtr );

				//std::cout << "Yes. \n";

				break;
			}
			case -1:{
				// No more movementPoints.
				// Move is put back in Path.
				playerPtr->push_front( unitId, from );
				//std::cout << "No, putting back Coord in Path. \n";
				break;
			}
			case -2:{
				// No merge possible on Tower.

				playerPtr -> removePath( unitId );

				MoveDeniedEvent * moveDeniedEvent = new MoveDeniedEvent( unitId, from );
				IEventDataPtr moveDeniedEventPtr( moveDeniedEvent );
				safeQueueEvent( moveDeniedEventPtr );

				//std::cout << "No, removing path. \n";

				break;
			}
			case -3:{
				// No merge possible between SeaUnits.

				playerPtr -> removePath( unitId );

				MoveDeniedEvent * moveDeniedEvent = new MoveDeniedEvent( unitId, from );
				IEventDataPtr moveDeniedEventPtr( moveDeniedEvent );
				safeQueueEvent( moveDeniedEventPtr );

				//std::cout << "No, removing path. \n";

				break;
			}
			case -4:{
				// Obstacle encountered. [Enemy unit or Wrong Terrain Type]

				playerPtr -> removePath( unitId );

				MoveDeniedEvent * moveDeniedEvent = new MoveDeniedEvent( unitId, from );
				IEventDataPtr moveDeniedEventPtr( moveDeniedEvent );
				safeQueueEvent( moveDeniedEventPtr );
				break;

				//std::cout << "No, removing path. \n";

			}
			case 1: {
				// Merge with Unit(s).
				// Move Event is created.
				MoveUnitEvent * moveUnitEvent = new MoveUnitEvent( playerId, unitId, from, to, 0 );
				IEventDataPtr moveUnitPtr( moveUnitEvent );
				safeQueueEvent( moveUnitPtr );

				//std::cout << "Yes. \n";

				break;
			}
			case 2: {
				// Merge with Structure.
				// Move Event is created.
				MoveUnitEvent * moveUnitEvent = new MoveUnitEvent( playerId, unitId, from, to, 1 );
				IEventDataPtr moveUnitPtr( moveUnitEvent );
				safeQueueEvent( moveUnitPtr );

				//std::cout << "Yes. \n";

				break;
			}
			case 3:
				// Attack Unit.
				// Attack Event is created.
				playerPtr -> removePath( unitId );

				attackSector(selectedTile, lastSelectedUnit);

				//std::cout << "Yes. Attack ! \n";

				break;
			case 4:
				// Attack Structure.
				// Attack Event is created.
				playerPtr -> removePath( unitId );

				attackSector(selectedTile, lastSelectedUnit);

				//std::cout << "Yes. Attack ! \n";

				break;
			case 5: {
				// Merge in Transport.
				// Move Event is created.
				playerPtr -> removePath( unitId );

				MoveUnitEvent * moveUnitEvent = new MoveUnitEvent( playerId, unitId, from, to, 2 );
				IEventDataPtr moveUnitPtr( moveUnitEvent );
				safeQueueEvent( moveUnitPtr );

				//std::cout << "Yes. Merge in transport ! \n";

				break;
			}
			default:

                //std::cout << "Uninterpreted case...\n";

				MoveDeniedEvent * moveDeniedEvent = new MoveDeniedEvent( unitId, from );
				IEventDataPtr moveDeniedPtr( moveDeniedEvent );
				safeQueueEvent( moveDeniedPtr );

				break;
		}

		return;

	}

	// Verification

	// --------------------------------------------------------
	// Is Terrain Type Matching Unit Type ?
	//
	// Verify if Unit with certain ObjectType can move over
	// a Hexagon with certain Terrain type.
	// --------------------------------------------------------
	bool GameEngine::isTerrainMatchingUnit( ObjectType ot, Terrain t ){

		bool isMatching;
		isMatching = false;

		if (ot == AIRFIGHTER || ot == BOMBER) {

			isMatching = true;

		}else if (ot == BATTLESHIP || ot == TRANSPORTER || ot == CARRIER || ot == SUBMARINE) {

			if (t == T_SEA || t == T_SEA2) {
				isMatching = true;
			}else {
				isMatching = false;
			}

		}else if (ot == ARTILLERY || ot == FLAG || ot == TANK){

			if (t == T_LAND || t == T_LAND2) {
				isMatching = true;
			}else {
				isMatching = false;
			}

		}

		return isMatching;
	}

	// --------------------------------------------------------
	// Is Hexagon Occupied by Enemies ?
	// --------------------------------------------------------
	bool GameEngine::isOccupiedByEnemies( HexagonPtr hp, int pId ){

		bool isOccupiedByEnemy;
		isOccupiedByEnemy = false;

		int i;
		i = 0;

		UnitPtr potentialEnemy;
		potentialEnemy = hp -> getUnitByPosition(i);

		ObjectType potentialSubmarineType;

		while ( potentialEnemy.get() != NULL ) {

			if ( ( potentialEnemy -> getPlayer() ) != pId ) {

				potentialSubmarineType = potentialEnemy -> getObjectType();

				if ( potentialSubmarineType == SUBMARINE) {

					// Can see enemy submarine be seen by actualPlayer ?
					if	( massEffect(hp, &GameEngine::isSonar, ( potentialEnemy -> getRadarRange() ) ) ){
						isOccupiedByEnemy = true;
						break;

					}

				} else {
					isOccupiedByEnemy = true;
					break;

				}


			}

			i++;

			potentialEnemy = hp -> getUnitByPosition(i);

		}

		return isOccupiedByEnemy;
	}

	// --------------------------------------------------------
	// Is Hexagon Occupied by Allied Sea Unit ?
	// --------------------------------------------------------
	bool GameEngine::isOccupiedByAlliedSeaUnit( HexagonPtr hp, int pId ){

		bool isSeaUnitAtDestination;
		isSeaUnitAtDestination = false;

		int i;
		i = 0;

		UnitPtr potentialSeaUnit;
		potentialSeaUnit = hp -> getUnitByPosition(i);

		ObjectType potentialSeaUnitType;

		while ( potentialSeaUnit.get() != NULL ) {

			if ( ( potentialSeaUnit -> getPlayer() ) == pId ) {

				potentialSeaUnitType = potentialSeaUnit -> getObjectType();

				if ( potentialSeaUnitType == BATTLESHIP || potentialSeaUnitType == TRANSPORTER ||
					potentialSeaUnitType == CARRIER || potentialSeaUnitType == SUBMARINE) {

					isSeaUnitAtDestination = true;
					break;
				}

			}

			i++;
			potentialSeaUnit = hp -> getUnitByPosition(i);

		}

		return isSeaUnitAtDestination;
	}

	// --------------------------------------------------------
	// Is Hexagon Occupied by Transport ?
	// --------------------------------------------------------
	bool GameEngine::isOccupiedByTransport( HexagonPtr hp ){

		bool isTransport;
		isTransport = false;

		int i;
		i = 0;

		UnitPtr potentialTransport;
		potentialTransport = hp -> getUnitByPosition(i);

		ObjectType potentialTransportType;

		while ( potentialTransport.get() != NULL ) {

			potentialTransportType = potentialTransport -> getObjectType();

			if ( potentialTransportType == TRANSPORTER ) {

				isTransport = true;
				break;
			}

			i++;
			potentialTransport = hp -> getUnitByPosition(i);

		}

		return isTransport;
	}


	// --------------------------------------------------------
	// Verify if move is possible
	//
	// Return values :
	//  5 : Merge in Transport.
	//  4 : Attack Structure.
	//  3 : Attack Unit.
	//  2 : Merge with Structure.
	//  1 : Merge with Unit(s).
	//  0 : Regular move.
	// -1 : No more movementPoints.
	// -2 : No merge possible on Tower.
	// -3 : No merge possible between SeaUnits.
	// -4 : Obstacle encountered.
	// --------------------------------------------------------
	int GameEngine::verifyMovability(bool isDestination){

		int returnCase;
		returnCase = 0;

		// Movement Points.
		if ( (lastSelectedUnit -> getRemainingMoves()) > 0) {

			ObjectType unitType = lastSelectedUnit -> getObjectType();
			Terrain hexagonType = selectedTile -> getTerrainType();

			StructurePtr sp = selectedTile -> getStructure();

			bool isStructure;
			isStructure = false;

			if (sp.get() != NULL) {
				isStructure = true;
			}

			if (isDestination) {
				// Attacks and merges are enabled.

				// Structure presence.
				if ( isStructure ) {
					if ( (sp -> getPlayer()) == (lastSelectedUnit -> getPlayer()) ) {

						if (sp -> getObjectType() == TOWER) {
							returnCase = -2; // No merge possible on Tower.

						} else {
							returnCase = 2; // Merge with Structure.

						}

					}else {
						returnCase = 4; // Attack Structure.

					}

				} else {
					if (selectedTile -> isOccupied()) {
						if ( isOccupiedByEnemies( selectedTile, ( lastSelectedUnit-> getPlayer() ) ) ) {

							returnCase = 3;

						}else {

							if ( isTerrainMatchingUnit(unitType, hexagonType) ) {

								if ( unitType != BATTLESHIP && unitType != TRANSPORTER &&
									unitType != CARRIER && unitType != SUBMARINE) {

									returnCase = 1; // Merge with Unit(s).

								} else {

									if ( isOccupiedByAlliedSeaUnit( selectedTile, ( lastSelectedUnit-> getPlayer() ) ) ) {
										returnCase = -3; // No merge possible between SeaUnits.

									} else {
										returnCase = 1; // Merge with Unit(s).

									}

								}

							} else {

								if ( isOccupiedByTransport( selectedTile ) && ( lastSelectedUnit -> getUnitClass() ) == UC_LAND ) {
									returnCase = 5; // Merge in Transport.

								} else {
									returnCase = -4; // Obstacle encountered.

								}

							}

						}

					} else {
						if (isTerrainMatchingUnit(unitType, hexagonType)) {
							returnCase = 0; // Regular move.

						} else {
							returnCase = -4; // Obstacle encountered.

						}

					}

				}


			}else {
				// No possible attack or merge since not destination.
				if ( isStructure ) {
					returnCase = -4; // Obstacle encountered.

				} else {
					if (selectedTile -> isOccupied()) {
						if ( isOccupiedByEnemies(selectedTile, ( lastSelectedUnit-> getPlayer() ) ) ) {
							returnCase = -4;

						} else {

							if ( isTerrainMatchingUnit(unitType, hexagonType) ) {

								if ( unitType != BATTLESHIP && unitType != TRANSPORTER &&
									unitType != CARRIER && unitType != SUBMARINE) {

									returnCase = 1; // Merge with Unit(s).

								} else {

									if ( isOccupiedByAlliedSeaUnit( selectedTile, ( lastSelectedUnit-> getPlayer() ) ) ) {
										returnCase = -3; // No merge possible between SeaUnits.

									} else {
										returnCase = 1; // Merge with Unit(s).

									}

								}

							} else {

								if ( isOccupiedByTransport( selectedTile ) && ( lastSelectedUnit -> getUnitClass() ) == UC_LAND ) {
									returnCase = 5; // Merge in Transport.

								} else {
									returnCase = -4; // Obstacle encountered.

								}

							}


						}

					} else {
						if (isTerrainMatchingUnit(unitType, hexagonType)) {
							returnCase = 0; // Regular move.

						} else {
							returnCase = -4; // Obstacle encountered.

						}

					}

				}

			}


		}else {
			returnCase = -1; // No more movementPoints
		}


		return returnCase;
	}


	// --------------------------------------------------------
	// Move Unit over one Hexagon.
	//
	// Move distribution to landMove(), seaMove() and
	// planeMove().
	//
	// Although the game state assumes a movement of one,
	// there is no check if movement is greater than one.
	// --------------------------------------------------------
	void GameEngine::move(int playerId, int unitId, Vector2 fromCoords, Vector2 toCoords, int mergeCase){

		PlayerPtr playerPtr = getSpecificPlayer(playerId);

        //std::cout << " --- Move Function --- \n";

		selectTile(fromCoords.x, fromCoords.y);

		if (selectedTile.get() == NULL) {
			//Move request failed - Could not find source tile.
			return;
		}

		selectUnit(playerId, unitId);

		if ( lastSelectedUnit.get() == NULL ) {
			//Move request failed - Could not find unit on source tile
			return;
		}

		selectTile(toCoords.x, toCoords.y);

		if (selectedTile.get() == NULL) {
			//Move request failed - Could not find destination tile.
			return;
		}

		// Task Distribution
		if (lastSelectedUnit -> getUnitClass() == UC_AIR) {
			airMove(toCoords.x, toCoords.y, mergeCase);

		} else if (lastSelectedUnit -> getUnitClass() == UC_SEA) {
			seaMove(playerId, toCoords.x, toCoords.y, mergeCase);

		} else {
			landMove(toCoords.x, toCoords.y, mergeCase);

		}

        //std::cout << " Path obtained from Move Function has size : " << actualPlayer->pathSize(unitId) << std::endl;

		if ( playerPtr->pathSize(unitId) > 1 ) {

            //std::cout << "Another Unitary move will be propagated from the Move Function...\n";

			//Trigger Move
			Vector2 curPos, nextPos;

			curPos = playerPtr->front(unitId);
			playerPtr->pop(unitId);

            //if( path.size() == actualPlayer->pathSize(unitId) )
			//  std::cout << "PROBLEM : Path was not modified :";

			nextPos =playerPtr->front(unitId);

            //if( nextPos.x == curPos.x && nextPos.y == curPos.y )
            //    std::cout << "The current position is the same as the next position.\n";

			if ( playerPtr->pathSize(unitId) == 1 ) {
				unitaryMove(playerId, unitId, curPos, nextPos, 1);

			}else {
				unitaryMove(playerId, unitId, curPos, nextPos, 0);

			}

		}else{

		    //std::cout << "Path is not enough long to produce a new move.\n";
			playerPtr->removePath(unitId);
		}

		return;
	}

	// --------------------------------------------------------
	// Move LandUnit over one tile
	//
	// Although the game state assumes a movement of one,
	// there is no check if movement is greater than one.
	// --------------------------------------------------------
	void GameEngine::landMove( int toX, int toY, int mergeCase ){

		int unitId;
		unitId = lastSelectedUnit -> getId();

		// Extraction
		bool isExtractionComplete;
		isExtractionComplete = lastSelectedTile -> remove( lastSelectedUnit );

		if ( !isExtractionComplete ) {
			// Must be inside a transporter... because check above found it.
			int transporterId;
			transporterId = isUnitTypeHere(lastSelectedTile, TRANSPORTER);

			actualPlayer -> removeTransportedUnit( transporterId, unitId );

		}

		StructurePtr sp;

		// Merge
		switch (mergeCase) {
			case 0:
				// Regular Merge.
				selectedTile -> addObject( lastSelectedUnit );
				break;

			case 1:
				// Structure merge.
				sp = selectedTile -> getStructure();

				sp -> pushGarrison( lastSelectedUnit );
				break;

			case 2:
				// In Transport Merge
				int transporterId;
				transporterId = isUnitTypeHere(selectedTile, TRANSPORTER);

				actualPlayer -> addTransportedUnit( transporterId, lastSelectedUnit );
				break;

			default:
				// Move Event Failed - Wrong mergeCase !
				break;
		}

		// Game state update
		lastSelectedUnit -> setLocation(toX,toY);

		//update moves left assuming single move
		int movesLeft;
		movesLeft = (lastSelectedUnit -> getRemainingMoves()) - 1;
		lastSelectedUnit -> setRemainingMoves( movesLeft );

		return;
	}

	// --------------------------------------------------------
	// Move AirUnit over one tile
	//
	// Although the game state assumes a movement of one,
	// there is no check if movement is greater than one.
	// --------------------------------------------------------
	void GameEngine::airMove( int toX, int toY, int mergeCase ){

		int unitId;
		unitId = lastSelectedUnit -> getId();

		// Extraction
		bool isExtractionComplete;
		isExtractionComplete = lastSelectedTile -> remove( lastSelectedUnit );

		if ( !isExtractionComplete ) {
			// Move Event Failed - Could not extract the Unit. Might be inside transport.
			return;
		}

		StructurePtr sp;
		AirUnitPtr aup( boost::static_pointer_cast<AirUnit>(lastSelectedUnit));//(AirUnit*) (*iterU).get() );
		// Merge
		switch (mergeCase) {
			case 0:
				// Regular Merge.
				selectedTile -> addObject( lastSelectedUnit );

				if ( isUnitTypeHere(selectedTile, CARRIER) > -1 ) {
					refuel(aup);
				}

				break;

			case 1:
				// Structure merge.
				sp = selectedTile -> getStructure();

				sp -> pushGarrison( lastSelectedUnit );
				refuel(aup);

				break;

			default:
				// Move Event Failed - Wrong mergeCase !
				break;
		}

		// Game state update
		lastSelectedUnit -> setLocation(toX,toY);

		//update moves left assuming single move
		int movesLeft;
		movesLeft = (lastSelectedUnit -> getRemainingMoves()) - 1;
		lastSelectedUnit -> setRemainingMoves( movesLeft );
		updateFuel(aup, selectedTile);


		return;
	}

	// --------------------------------------------------------
	// Move SeaUnit over one tile
	// --------------------------------------------------------
	void GameEngine::seaMove( int playerId, int toX, int toY, int mergeCase ){

		PlayerPtr p = getSpecificPlayer(playerId);

		int unitId;
		unitId = lastSelectedUnit -> getId();

		// Extraction
		bool isExtractionComplete;
		isExtractionComplete = lastSelectedTile -> remove( lastSelectedUnit );
        //std::cout<<"\nExtraction Complete: "<<isExtractionComplete;
        if ( !isExtractionComplete ) {
			// Move Event Failed - Could not extract the Unit. Might be inside transport.
			return;

		}

		//std::cout << "SeaMove MergeCase: " << mergeCase << std::endl;

		// Merge
		if (mergeCase == 0) {
			// Regular Merge.
			selectedTile -> addObject( lastSelectedUnit );

			if ( lastSelectedUnit -> getObjectType() == TRANSPORTER ){

                //Generate same MoveEvent for all transported units.
                int transporterId;
				transporterId = isUnitTypeHere(selectedTile, TRANSPORTER);
				std::list<UnitPtr> carry = p -> getTransportedUnitList( transporterId );

				//std::cout << "Size of carry: " << carry.size() << std::endl;

				if (!carry.empty()) {
					std::list<UnitPtr>::iterator iter;

					Vector2 from;
					Vector2 to;

					for (iter = carry.begin(); iter != carry.end(); iter++){

						from.x = lastSelectedTile -> getX();
						from.y = lastSelectedTile -> getY();
						to.x = selectedTile -> getX();
						to.y = selectedTile -> getY();

						MoveUnitEvent * moveUnitEvent = new MoveUnitEvent( playerId, (*iter)->getId(), from, to, 2 );
						IEventDataPtr moveUnitPtr( moveUnitEvent );
						safeQueueEvent( moveUnitPtr );
					}
				}

            }

		}else if (mergeCase == 1) {
			// Structure merge.
			StructurePtr sp;
			sp = selectedTile -> getStructure();

			// If merging Transporter in Structure,
			// push all transported Units in garrison.
			if ( ( lastSelectedUnit -> getObjectType() ) == TRANSPORTER ) {
				int transporterId;
				transporterId = isUnitTypeHere(selectedTile, TRANSPORTER);
				std::list<UnitPtr> carry = p -> clearTransport( transporterId );

				Vector2 from;
				Vector2 to;

				while ( !carry.empty() ) {
					UnitPtr carried = carry.front();
					carry.pop_front();

					from.x = lastSelectedTile -> getX();
					from.y = lastSelectedTile -> getY();
					to.x = selectedTile -> getX();
					to.y = selectedTile -> getY();

					MoveUnitEvent * moveUnitEvent = new MoveUnitEvent( playerId, (carried)->getId(), from, to, 1 );
					IEventDataPtr moveUnitPtr( moveUnitEvent );
					safeQueueEvent( moveUnitPtr );

				}

			}

			sp -> pushGarrison( lastSelectedUnit );
		}else {
			// Move Event Failed - Wrong mergeCase !
		}

		// Game state update
		lastSelectedUnit -> setLocation(toX,toY);

		//update moves left assuming single move
		int movesLeft;
		movesLeft = (lastSelectedUnit -> getRemainingMoves()) - 1;
		lastSelectedUnit -> setRemainingMoves( movesLeft );

		return;
	}


	//TURN UPDATES

	// --------------------------------------------------------
	// Start Turn Updates
	//
	// Automatically called upon endTurnUpdates() end of
	// function.
	//
	// Generates startTurnEvent, destroyedUnitEvent, and
	// createdUnitEvent.
	//
	// Current procedure makes unitUpdates and structureUpdates
	// for each actualPlayer objects.
	//
	// --------------------------------------------------------
	void GameEngine::startTurnUpdates(){

       // std::cout << " --- Start Turn Updates --- \n";

		BeginTurnEvent * beginTurn = new BeginTurnEvent( actualPlayer -> getPlayerId() );
		IEventDataPtr beginTurnPtr( beginTurn );
		safeQueueEvent(beginTurnPtr);

		//skip default player
		if ( (actualPlayer -> getPlayerId()) == 0) {
			endTurnUpdates();
			return;
		}

		//skip gameOver player
		if ( (actualPlayer -> getGameOver()) == 1) {
			endTurnUpdates();
			return;
		}

		// Unit updates
		// - Check if hp = 0 => destroy
		// - Give back full movement points
		// - Plane needs to updateFuel up or down (see conditions)
		std::list<UnitPtr>& lUnitOwned = actualPlayer -> getUnitList();

		if (!lUnitOwned.empty()) {

			std::list<UnitPtr>::iterator iterU;
			for (iterU = lUnitOwned.begin(); iterU != lUnitOwned.end(); iterU++) {
				UnitPtr up = (*iterU);

				HexagonPtr hp = getHexagon((*iterU)->getX(), (*iterU)->getY());
				if (hp.get() == NULL)
					continue;
				//Check if hp = 0 => destroy from player's list and hexagon's list
				if (up.get() == NULL){

					//delete null pointer from player's list
					iterU = lUnitOwned.erase(iterU);

				} else {

					if ( up->getRemainingHitpoints() == 0 ) {

						//delete from Hexagon
						hp -> remove( up );

						//delete from player's list
						iterU = lUnitOwned.erase(iterU);

						UnitDestroyedEvent * unitDestroyed = new UnitDestroyedEvent( (*iterU) -> getId() );
						IEventDataPtr unitDestroyedPtr( unitDestroyed );
						safeQueueEvent(unitDestroyedPtr);


					} else {

						// - Plane needs to updateFuel => check if plane is inside structure or over a carrier
						if ( (up -> getObjectType()) == AIRFIGHTER || (up -> getObjectType()) == BOMBER ) {
							if (up -> getRemainingMoves() < up -> getMaxMoves()) {
								AirUnitPtr aup( boost::static_pointer_cast<AirUnit>((*iterU)));//(AirUnit*) (*iterU).get() );
								updateFuel(aup, hp);
							}
						}

						// - Give back full movement points and acted
						if( !up->isFortified() ){
							resetUnit(up);
						}
						else{
							up -> setActed( false );
							up-> setRepaired( false );
						}

					}

				}

			}
		}

		// Structure updates :
		// - Check for production requests due
		std::list<StructurePtr> lStructureOwned = actualPlayer -> getStructureList();
		//list cannot be empty => if empty, the player has "gameOver" set to true.

		int sx;
		int sy;
		int pId;

		std::list<StructurePtr>::iterator iterS;
		for (iterS = lStructureOwned.begin(); iterS != lStructureOwned.end(); iterS++) {
			StructurePtr sp = (*iterS);

			std::list<ProductionRequestPtr> lProductionRequestDue = sp->startTurnProductionUpdate();

			if (lProductionRequestDue.empty() == false) {
				//get structure info
				pId = sp->getPlayer();
				sx = sp->getX();
				sy = sp->getY();

				//process requests completion
				std::list<ProductionRequestPtr>::iterator iterP;
				for( iterP = lProductionRequestDue.begin(); iterP != lProductionRequestDue.end(); ++iterP){
					ProductionRequestPtr prp = (*iterP);
					UnitPtr up = generateUnit( pId, prp, sx, sy );

					//add unit to player's list
					actualPlayer -> addUnit(up);

					//add unit to structure's garrison
					sp -> pushGarrison(up);
                    resetUnit(up);

					ObjectParams * o_params = new ObjectParams;
					o_params -> playerId = pId;
					o_params -> objectId = up -> getId();
					o_params -> x_coord = sx;
					o_params -> y_coord = sy;
					o_params -> type = up->getObjectType();

					//printf("Create Event for unit #%d", up -> getId());

					CreateObjectEvent * createEvent = new CreateObjectEvent( o_params );
					IEventDataPtr createEventPtr( createEvent );
					safeQueueEvent(createEventPtr);
				}
			}


		}//end of for loop over all structures

		return;
	}

	// --------------------------------------------------------
	// Retrieves list of structures still free to build a unit
	//
	// Intended to be eventually triggered by GraphicsEngine.
	// --------------------------------------------------------
	std::list<int> GameEngine::getFreeToBuildStructures(){

		std::list<int> sIdList;

		std::list<StructurePtr> sl = actualPlayer -> getStructureList();
		std::list<StructurePtr>::iterator iter;

		if (!sl.empty()) {
			int actualTurn = actualPlayer -> getTurnNumber();

			for (iter = sl.begin(); iter != sl.end(); ++iter) {
				StructurePtr sp = (*iter);
				if ( sp -> isFreeToBuild(actualTurn) ) {
					sIdList.push_back(sp -> getId());
				}
			}
		}

		return sIdList;
	}

	// --------------------------------------------------------
	// Verify if any actualPlayer structures are still free to
	// build a unit.
	//
	// Triggered by EndTurnRequestEvent - verifications
	// --------------------------------------------------------
	bool GameEngine::isFreeToBuildStructure(){

		std::list<StructurePtr> sl = actualPlayer -> getStructureList();
		std::list<StructurePtr>::iterator iter;

		bool anyFreeToBuildStructure = false;
		int actualTurn = actualPlayer -> getTurnNumber();

		for (iter = sl.begin(); iter != sl.end(); ++iter) {
			StructurePtr sp = (*iter);
			if ( sp -> isFreeToBuild(actualTurn) ) {
				anyFreeToBuildStructure = true;
				break;
			}
		}

		return anyFreeToBuildStructure;
	}

	// --------------------------------------------------------
	// Retrieves list of movable units for actualPlayer
	//
	// Intended to be eventually triggered by GraphicsEngine.
	// --------------------------------------------------------
	std::list<int> GameEngine::getMovableUnits(){

		std::list<int> uIdList;

		std::list<UnitPtr> ul = actualPlayer -> getUnitList();
		std::list<UnitPtr>::iterator iter;

		if (!ul.empty()) {

			for (iter = ul.begin(); iter != ul.end(); iter++) {
				UnitPtr up = (*iter);
				if (up -> getRemainingMoves() != 0 && up -> getActed() == false) {
					uIdList.push_back(up -> getId());
				}
			}
		}

		return uIdList;
	}

	// --------------------------------------------------------
	// Verify if any actualPlayer units are still movable.
	//
	// Triggered by EndTurnRequestEvent - verifications
	// --------------------------------------------------------
	bool GameEngine::isMovableUnits(){

		std::list<UnitPtr> ul = actualPlayer -> getUnitList();
		std::list<UnitPtr>::iterator iter;

		bool anyMovableUnit = false;

		if (!ul.empty()) {
			for (iter = ul.begin(); iter != ul.end(); ++iter) {
				UnitPtr up = (*iter);
				if (up -> getRemainingMoves() != 0 && up -> getActed() == false) {
					anyMovableUnit = true;
					break;
				}
			}
		}

		return anyMovableUnit;

	}

	// --------------------------------------------------------
	// End Turn Updates
	//
	// Triggered by EndTurnRequestEvent
	// --------------------------------------------------------
	void GameEngine::endTurnUpdates(){

		if ( (actualPlayer -> getPlayerId()) != 0 ) {


            //std::cout << " --- End Turn Updates for Player #" << actualPlayer -> getPlayerId() <<"--- \n";

			// Check for unfinished paths.
			std::list<UnitPtr> lUnitsOwned = actualPlayer -> getUnitList();

			if ( !lUnitsOwned.empty() ){

			    std::list<UnitPtr>::iterator iterU;
			    for( iterU = lUnitsOwned.begin(); iterU != lUnitsOwned.end(); iterU++) {

					int unitId = (*iterU)->getId();
					if( !actualPlayer->isPath( unitId ) ){
						continue;
					}

					Vector2 curPos, nextPos;



					curPos = actualPlayer->front(unitId);
					actualPlayer->pop(unitId);

					nextPos = actualPlayer->front(unitId);

					// Check if it is the last move on the path.
					if (actualPlayer->pathSize(unitId) == 1) {

						unitaryMove(actualPlayer -> getPlayerId(), unitId, curPos, nextPos, 1);

					}else {

						unitaryMove(actualPlayer -> getPlayerId(), unitId, curPos, nextPos, 0);

					}

				}

			}



			std::list<StructurePtr> lStructureOwned = actualPlayer -> getStructureList();

			if (!lStructureOwned.empty()) {

				//update production requests and underAttack field
				std::list<StructurePtr>::iterator iterS;
				for (iterS = lStructureOwned.begin(); iterS != lStructureOwned.end(); iterS++) {

					StructurePtr sp = (*iterS);

					//underAttack reset
					if ( sp -> isUnderAttack() ) {
						sp -> setUnderAttack( false );
					}else{
						//update Production only if not underAttack
						sp -> endTurnProductionUpdate();
					}
				}
			}


		}

		//turn number increments
		actualPlayer->incrementTurnNumber();

		++totalTurnCompleted;

		actualPlayerIndex = totalTurnCompleted%playerId;

		actualPlayer = playerList.at(actualPlayerIndex);

		startTurnUpdates();

		return;
	}


	// --------------------------------------------------------
	// Is some player GameOver ?
	//
	// Verify if the player has at least one structure.
	//
	// Generates Game Over Event.
	// --------------------------------------------------------
	int GameEngine::isGameOver( int playerId ){

		PlayerPtr pp = getSpecificPlayer(playerId);

		if ( pp.get() == NULL ) {
			std::cerr << "GameOver check failed. - Could not get player.\n";
			return 0;
		}

		if ( pp -> getGameOver() == 1 ) {

			//He is already gameover.
			return 1;

		}else {
			int nTower;
			nTower = pp -> getNTower();
			if ( nTower == 0 ) {

				//Player has no more towers.
				pp -> setGameOver(1);

				GameOverEvent * gameOverEvent = new GameOverEvent( playerId );
				IEventDataPtr gameOverEventPtr( gameOverEvent );
				safeQueueEvent(gameOverEventPtr);

				bool isEnd;
				isEnd = isEndGame();

				if (!isEnd) {
					// We might want to allow the last loser
					// to look at the map with his units and
					// enemy units to compare if it was a
					// close or not.
					killPlayerUnits( pp );
				}

				return 1;
			}else {

				//Player is still alive.
				return 0;

			}

		}

	}

	// --------------------------------------------------------
	// Terminates Player's units
	//
	// Used by GameOver when there is no endGame
	// --------------------------------------------------------
	void GameEngine::killPlayerUnits( PlayerPtr &pp ){

		std::list<UnitPtr>& unitPtrList = pp -> getUnitList();

		if (!unitPtrList.empty()) {

			//remove each unit from their hexagon
			std::list<UnitPtr>::iterator iter;
			for( iter = unitPtrList.begin(); iter != unitPtrList.end(); ++iter){
				UnitPtr up = (*iter);

				HexagonPtr hp = getHexagon( up -> getX(), up -> getY() );

				hp -> remove( up );

			}

			//clear player's unit list
			unitPtrList.clear();
		}

		return;
	}

	// --------------------------------------------------------
	// Terminates Player's units
	//
	// Verify if there is only one player left which is not
	// GameOver.
	//
	// Generates End Game Event.
	// --------------------------------------------------------
	bool GameEngine::isEndGame(){

		//skips 0th player
		int i = 1;

		int aliveCount = 0;

		PlayerPtr pp;
		int winnerId;

		//count non-GameOver players
		for(; i < playerId; i++){
			pp = getSpecificPlayer(i);

			//this check can be deleted after testing and if we don't delete players in the game
			if ( pp.get() == NULL ) {
				std::cerr << "End Game check Failed. - Could not get the player.\n";
				return false;
			}

			if ( pp -> getGameOver() == 0 ){
				winnerId = pp -> getPlayerId();
				++aliveCount;
			}

		}

		//verify if there is more than one player alive
		if ( aliveCount < 2 ){

			EndGameEvent * endGameEvent = new EndGameEvent( winnerId );
			IEventDataPtr endGameEventPtr( endGameEvent );
			safeQueueEvent(endGameEventPtr);
			return true;
		}

		return false;
	}

	const char * GameEngine::GetName(void) {
		return "GameEngineName";
	}

	//All events related to logical interaction are handled here
	bool GameEngine::HandleEvent( IEventData & event ) {

		EventType evtType = event.VGetEventType();

		//ordered by highest probability of being called
		if ( evtType == MoveRequestEvent::sk_EventType ) {

			//printf("Move Request Event");
			MoveRequestEvent & moveRequestEvent = static_cast<MoveRequestEvent &>(event);
			//Verify and generate MoveEvent/AttackEvent if possible
			requestMove( moveRequestEvent.m_UnitId, moveRequestEvent.m_Position, moveRequestEvent.m_Destination);
			return false;

		}else if( evtType == MoveUnitEvent::sk_EventType ) {

			//printf("Move Unit Event");
			MoveUnitEvent & moveUnitEvent = static_cast<MoveUnitEvent &>(event);
			move( moveUnitEvent.m_PlayerId, moveUnitEvent.m_UnitId, moveUnitEvent.m_Position, moveUnitEvent.m_Destination, moveUnitEvent.m_MergeCase );
			return false;

		}else if( evtType == AttackEvent::sk_EventType ) {

			//printf("Attack Unit Event");
			AttackEvent & attackEvent = static_cast<AttackEvent &>(event);
			UnitPtr attacker = getUnitPointer( attackEvent.m_AttackerId, attackEvent.m_PositionAttacker );
			
			if (attacker.get() == NULL) {
				//std::cout << "Attack Failed - Could not find the Attacker." << std::endl;
				return false;
			}
			
			UnitPtr defender = getUnitPointer( attackEvent.m_DefenderId, attackEvent.m_PositionDefender );
			
			if (defender.get() == NULL) {
				//std::cout << "Attack Failed - Could not find the Defender." << std::endl;
				return false;
			}
			
			battleUnit(defender, attacker);
			return false;

		}else if(  evtType == BuildRequestEvent::sk_EventType ) {

			//printf("Build Request Event");
			BuildRequestEvent & buildRequestEvent = static_cast<BuildRequestEvent &>(event);
			requestBuild(buildRequestEvent.m_StructureId, buildRequestEvent.m_Position, buildRequestEvent.m_Type);
			return false;

		}else if(  evtType == BuildEvent::sk_EventType ) {

			//printf("Build Event");
			//std::cout << event.VSerialize();
			BuildEvent & buildEvent = static_cast<BuildEvent &> (event);
			build( buildEvent.m_StructureId, buildEvent.m_Position, buildEvent.m_UnitId, buildEvent.m_Type );
			objectId++;
			return false;

		}else if( evtType == BuildCancelledEvent::sk_EventType ) {

			//printf("Build Cancelled Event");
			BuildCancelledEvent & buildCancelledEvent = static_cast<BuildCancelledEvent &> (event);
			cancelBuild( buildCancelledEvent.m_StructureId, buildCancelledEvent.m_Position, buildCancelledEvent.m_UnitId );
			return false;

		}else if(  evtType == BeginTurnEvent::sk_EventType ) {

			//printf("Begin Turn Event");
			// Nothing automatically called by EndTurnUpdates.
			return false;

		}else if(  evtType == EndTurnEvent::sk_EventType ) {

			//printf("End Turn Event");
			endTurnUpdates();
			return false;

		}else if(  evtType == FortifyRequestEvent::sk_EventType ) {
			//printf("Fortify Request Event");
			FortifyRequestEvent & fortifyRequestEvent = static_cast<FortifyRequestEvent &>(event);
			UnitPtr unitToFortify = getUnitPointer( fortifyRequestEvent.m_UnitId, fortifyRequestEvent.m_Position );

			if (actualPlayer -> getPlayerId() != playerID) {
				FortifyDeniedEvent * denyFortifyUnit = new FortifyDeniedEvent( fortifyRequestEvent.m_UnitId, fortifyRequestEvent.m_Position, fortifyRequestEvent.m_Fortify );
				IEventDataPtr denyFortifyPtr( denyFortifyUnit );
				safeQueueEvent( denyFortifyPtr );
			}else {

				if ( canAct( unitToFortify ) )
				{
					FortifyEvent * fortifyUnit = new FortifyEvent( fortifyRequestEvent.m_UnitId, fortifyRequestEvent.m_Position, fortifyRequestEvent.m_Fortify );
					IEventDataPtr fortifyPtr( fortifyUnit );
					safeQueueEvent( fortifyPtr );
				}else
				{
					FortifyDeniedEvent * denyFortifyUnit = new FortifyDeniedEvent( fortifyRequestEvent.m_UnitId, fortifyRequestEvent.m_Position, fortifyRequestEvent.m_Fortify );
					IEventDataPtr denyFortifyPtr( denyFortifyUnit );
					safeQueueEvent( denyFortifyPtr );
				}
			}
			return false;

		}else if(  evtType == FortifyEvent::sk_EventType ) {
			//printf("Fortify Event");
			FortifyEvent & fortifyEvent = static_cast<FortifyEvent &>(event);
			UnitPtr unitToFortify = getUnitPointer( fortifyEvent.m_UnitId, fortifyEvent.m_Position );
			fortify( unitToFortify, fortifyEvent.m_Fortify );
			return false;

		}else if(  evtType == ShellRequestEvent::sk_EventType ) {
			//printf("Shell request Event");

			ShellRequestEvent & shellRequestEvent = static_cast<ShellRequestEvent &>(event);
			UnitPtr unitShelling = getUnitPointer( shellRequestEvent.m_ShellerId, shellRequestEvent.m_PositionSheller );

			if (actualPlayer -> getPlayerId() != playerID){
				ShellDeniedEvent * deniedShellUnit = new ShellDeniedEvent( shellRequestEvent.m_ShellerId, shellRequestEvent.m_PositionShelley );
				IEventDataPtr shellPtr( deniedShellUnit );
				safeQueueEvent( shellPtr );
			}else{
				if ( canAct( unitShelling ) && ( unitShelling->getAbilities() & ABILITY_SHELL ) )
				{
					ShellEvent * shellUnit = new ShellEvent( shellRequestEvent.m_ShellerId, shellRequestEvent.m_PositionSheller, shellRequestEvent.m_ShelleyId, shellRequestEvent.m_PositionShelley );
					IEventDataPtr shellPtr( shellUnit );
					safeQueueEvent( shellPtr );
				}else
				{
					ShellDeniedEvent * deniedShellUnit = new ShellDeniedEvent( shellRequestEvent.m_ShellerId, shellRequestEvent.m_PositionShelley );
					IEventDataPtr shellPtr( deniedShellUnit );
					safeQueueEvent( shellPtr );
				}
			}
			return false;

		}else if(  evtType == ShellEvent::sk_EventType ) {
			//printf("Shell Event");

			ShellRequestEvent & shellEvent = static_cast<ShellRequestEvent &>(event);
			UnitPtr unitShelling = getUnitPointer( shellEvent.m_ShellerId, shellEvent.m_PositionSheller );
			UnitPtr unitShelled = getUnitPointer( shellEvent.m_ShelleyId, shellEvent.m_PositionShelley );
			shell( unitShelled, unitShelling );

			return false;

		}else if(  evtType == RepairRequestEvent::sk_EventType ) {
			//printf("Repair request Event");

			RepairRequestEvent & repairRequestEvent = static_cast<RepairRequestEvent &>(event);
			UnitPtr unitToRepair = getUnitPointer( repairRequestEvent.m_UnitId, repairRequestEvent.m_Position );

			if (actualPlayer -> getPlayerId() != playerID){

				RepairDeniedEvent * deniedRepairUnit = new RepairDeniedEvent( repairRequestEvent.m_UnitId, repairRequestEvent.m_Position );
				IEventDataPtr denyRepairPtr( deniedRepairUnit );
				safeQueueEvent( denyRepairPtr );

			}else{
				if( !unitToRepair->getRepaired() ){
					RepairEvent * repairUnit = new RepairEvent( repairRequestEvent.m_UnitId, repairRequestEvent.m_Position );
					IEventDataPtr repairPtr( repairUnit );
					safeQueueEvent( repairPtr );
				}else
				{
					RepairDeniedEvent * deniedRepairUnit = new RepairDeniedEvent( repairRequestEvent.m_UnitId, repairRequestEvent.m_Position );
					IEventDataPtr denyRepairPtr( deniedRepairUnit );
					safeQueueEvent( denyRepairPtr );
				}
			}

			return false;
		}else if(  evtType == RepairEvent::sk_EventType ) {
			//printf("Repair Event");

			RepairEvent & repairEvent = static_cast<RepairEvent &>(event);
			UnitPtr unitToRepair = getUnitPointer( repairEvent.m_UnitId, repairEvent.m_Position );
			repair( unitToRepair );

			return false;
		}else if(  evtType == BombRequestEvent::sk_EventType ) {
			//printf("Bomb Request Event");

			BombRequestEvent & bombRequestEvent = static_cast<BombRequestEvent &>(event);
			UnitPtr bomber = getUnitPointer( bombRequestEvent.m_UnitId, bombRequestEvent.m_PositionUnit );

			if (actualPlayer -> getPlayerId() != playerID){
				BombDeniedEvent * deniedBomb = new BombDeniedEvent( bombRequestEvent.m_UnitId, bombRequestEvent.m_PositionUnit );
				IEventDataPtr denyBombPtr( deniedBomb );
				safeQueueEvent( denyBombPtr );
			}else{
			HexagonPtr bHex = getHexagon( bombRequestEvent.m_PositionUnit );
			if( !bomber->getActed() && ( bomber->getAbilities() & ABILITY_BOMB ) && massEffect( bHex, &GameEngine::isStructure, 1 ) ){
			    BombEvent * bomb = new BombEvent( bombRequestEvent.m_UnitId, bombRequestEvent.m_PositionUnit, bombRequestEvent.m_StructureId, bombRequestEvent.m_PositionStructure );
				IEventDataPtr bombPtr( bomb );
				safeQueueEvent( bombPtr );
			}
			else{
			    BombDeniedEvent * deniedBomb = new BombDeniedEvent( bombRequestEvent.m_UnitId, bombRequestEvent.m_PositionUnit );
				IEventDataPtr denyBombPtr( deniedBomb );
				safeQueueEvent( denyBombPtr );
			}
			}

			return false;
		}else if(  evtType == BombEvent::sk_EventType ) {
			//printf("Bomb Event");

			BombEvent & bombEvent = static_cast<BombEvent &>(event);
			HexagonPtr bHex = getHexagon( bombEvent.m_PositionStructure );
			massEffect( bHex, &GameEngine::bomb, 1 );

			return false;
		}else if(  evtType == ConquerEvent::sk_EventType ) {
			//printf("Conquer Event");
			ConquerEvent & conquerEvent = static_cast<ConquerEvent &>(event);
			UnitPtr unitConquering = getUnitPointer( conquerEvent.m_UnitId, conquerEvent.m_PositionUnit );
			HexagonPtr hex = getHexagon( conquerEvent.m_PositionStructure );
			StructurePtr structureConquered = hex->getStructure();
			conquer( unitConquering, structureConquered );
			return false;
		}else if(  evtType == SaveGameEvent::sk_EventType ) {
			//printf("SaveGame Event");
			return false;
		}else if(  evtType == GameOverEvent::sk_EventType ) {
			//printf("GameOver Event");
			// Nothing because local attacks.
			return false;
		}else if(  evtType == EndGameEvent::sk_EventType ) {
			//printf("EndGame Event");
			// Nothing because local attacks.
			return false;
		}


		return false;
	}

    void GameEngine::printPath( std::list< Vector2 > path ){

		std::list< Vector2 >::iterator it;
		std::ostringstream o;

		o << "Path is : ";

		for( it = path.begin(); it != path.end(); it++){

			o << "( ";
			o << (*it).x;
			o << " , ";
			o << (*it).y;
			o << " ) ; ";

		}

		return;
    }


    void GameEngine::printPlayer(int playerId){

        PlayerPtr pp = getSpecificPlayer( playerId );

        if (pp.get() != NULL) {
			std::cout << "\n\n\n**********\n-> Displaying Player #" << playerId << "'s stats :\n";
			std::cout << pp -> getStats();
			std::cout << "\n**********\nEND OF STATS for Player  #" << playerId << "\n**********\n\n\n";
        }else {
			std::cout << "Could not display player's stats.\n";

        }

        return;
    }

}
