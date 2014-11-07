/*
 *  TestProductionLogic.cpp
 *  IslandWars
 *
 *  Created by Patrick Desmarais on 10-03-12.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include <iostream>
#include <vector>
#include "TestProductionLogic.h"
#include "Player.h"
#include "Hexagon.h"

using namespace IslandWars;

//used by Production
void passProductionTurn(ProductionPtr &p, std::list<UnitPtr> &lUnit, int &turn, HexagonPtr hp){
	/*
	std::cout << "End of turn...\n";
	int check = p -> endTurnUpdate();
	std::cout << "Requests to check : " << check << ".\n";
	
	turn++;
	
	std::cout << "Start of turn : " << turn << "\n";
	lUnit = p->startTurnUpdate();//1, hp);
	
	if ((int) lUnit.size() > 0) {
		std::cout << "We have " << lUnit.size() <<" unit(s) to produce!!!\n";
	}else {
		std::cout << "No units to produce...\n";
	}
     */
	
	return;
}

//Fake GameEngine
class GameEngine {
private:
	int objectId;
	int playerId;
	
	//Note: There will always be a player 0 for unknown structures
	std::vector<PlayerPtr> playerList;
	int actualPlayerIndex;
	
public:
	GameEngine();
	~GameEngine();
	
	int getActualPlayerTurn();
	
	//Feed in an empty pointer to be initilized upon found success
	int getSpecificPlayer(PlayerPtr &pp, int playerId);
	
	void createPlayer(std::string name);
	//Get the HexagonPtr in the HexagonMap according to its x-y coords
	void createStructure(int playerId, int xCoords, int yCoords, ObjectType);
	//Overload the one above
	void createStructure(int playerId, HexagonPtr &hp, ObjectType type);
	
	int handleProductionRequest(int playerId, int structureId, ObjectType type);
	int cancelProductionRequest(int playerId, int structureId, ObjectType type, int cost);
	
	void acquireStructure( int fromPlayer, int toPlayer, int structureId );
	
	void startTurnUpdates();
	void endTurnUpdates();
};

GameEngine::GameEngine(){
	objectId = 0;
	playerId = 0;
	actualPlayerIndex = 0;
}

GameEngine::~GameEngine(){
	
}

int GameEngine::getActualPlayerTurn(){
	return actualPlayerIndex%playerId;
}

int GameEngine::getSpecificPlayer(PlayerPtr &pp, int playerId){
	if ( playerId >= playerList.size() ) {
		return 0;
	}
	pp = playerList.at(playerId);
	return 1;
}

void GameEngine::createPlayer(std::string name){
	//createPlayer
	PlayerPtr pp( new Player(playerId, name) );
	
	//add it to list
	playerList.push_back(pp);
	
	//playerId number is incremented
	++playerId;
	
	return;
}

//FOR THE MOMENT DOES NOTHING !!! - NEEDS HEXAGON_MAP
//Get the HexagonPtr in the HexagonMap according to its x-y coords
void createStructure(int playerId, int xCoords, int yCoords, ObjectType){
	
	return;
}

void GameEngine::createStructure(int playerId, HexagonPtr &hp, ObjectType type){
    return;
    /*
	//get player pointer
	PlayerPtr pp;
	int found = getSpecificPlayer(pp, playerId);
	
	if ( !found ){
		std::cout << "The player you requested does not exist !\n";
		return;
	}
	
	//Create parameters to pass to factory
	ObjectParams* op = new ObjectParams();
	
	op->playerId = playerId;
	op->objectId = objectId;
	op->hexagon = hp;
	op->type = type;
	
	//Call factory to live
	ObjectFactory* of = new ObjectFactory();
	
	//Call factory to work
	StructurePtr sp(of->createStructure(op));
	
	//Cleaning
	free(op);
	free(of);
	
	//std::cout << "I created a structure of id " << sp->getId() << " for player id " << pp->getPlayerId() << ".\n";
	
	//add to player's list
	pp->addStructure(sp);
	
	++objectId;
	
	return;
     */
}

//instead of just returning 0, it could create response messages accordingly.
int GameEngine::handleProductionRequest(int playerId, int structureId, ObjectType type){
	return 1;
    /*
	//not this player's turn?
	//std::cout << "I'm feeding playerId: " << playerId << ", but it is actually" << getActualPlayerTurn() <<"!\n";
	if (playerId != getActualPlayerTurn() ) {
		std::cout << "It's not your turn, big !\n";
		return 0;
	}
	
	//get player pointer
	PlayerPtr pp;
	int found = getSpecificPlayer(pp, playerId);
	
	if ( !found ){
		std::cout << "The player you requested does not exist !\n";
		return 0;
	}
	
	//get structure pointer from player's list
	StructurePtr sp;
	int found2 = pp -> getStructure(structureId, sp);
	
	if (!found2) {
		std::cout << "The structure you requested does not exist in the player's list !\n";
		return 0;
	}
	
	//get the actual turn number
	int turn = pp -> getTurnNumber();
	
	//Try to process this request...
	int result = sp -> handleProductionRequest(type, turn);
	
	if (result == 0) {
		std::cout << "Couldn't add your request to this structure... (full)\n";
		return 0;
	}
	
	//finally...
	std::cout << "Your request was processed successfully...\n";
	return 1;
     */
}

int GameEngine::cancelProductionRequest(int playerId, int structureId, ObjectType type, int cost){
    return 1;
    /*
	//not this player's turn?
	if (playerId != getActualPlayerTurn()) {
		std::cout << "It's not your turn, big !\n";
		return 0;
	}
	
	//get player pointer
	PlayerPtr pp;
	int found = getSpecificPlayer(pp, playerId);
	
	if ( !found ){
		std::cout << "The player you requested does not exist !\n";
		return 0;
	}
	
	//get structure pointer from player's list
	StructurePtr sp;
	int found2 = pp -> getStructure(structureId, sp);
	
	if (!found2) {
		std::cout << "The structure you requested does not exist in the player's list !\n";
		return 0;
	}
	
	sp->cancelProductionRequest(type, cost);
	
	//finally...
	return 1;
     */
}

void GameEngine::acquireStructure( int fromPlayer, int toPlayer, int structureId ){
	return;
    /*
	PlayerPtr frompp;
	int i = getSpecificPlayer(frompp, fromPlayer);
	
	if (!i) {
		std::cout << "Couldn't find the player with this id...\n";
		return;
	}
	
	PlayerPtr topp;
	i = getSpecificPlayer(topp, toPlayer);
	if (!i) {
		std::cout << "Couldn't find the player with this id...\n";
		return;
	}
	
	//get structure pointer from player's list
	StructurePtr sp;
	int found2 = frompp -> getStructure(structureId, sp);
	
	if (!found2) {
		std::cout << "The structure you requested does not exist in the player's list !\n";
		return;
	}
	
	//humiliation in front of fromPlayer
	sp->setPlayer( toPlayer );
	topp->addStructure(sp);
	
	//now fromPlayer loses it, but make sure his enemy get no production free
	sp->clearProduction();
	i = frompp->deleteStructure( structureId );
	
	if (!i) {
		std::cout << "Couldn't find the structure anymore...\n";
		return;
	}
	return;
     */
}

void GameEngine::startTurnUpdates(){
    return;
    /*
	//At the beginning of a turn,
	//1. check for production requests due in each structure
	
	PlayerPtr pp;
	int i = getSpecificPlayer(pp, getActualPlayerTurn());
	
	if (!i) {
		std::cout << "You have a logical problem in your code, dude.\n";
		return;
	}
	
	std::list<StructurePtr> lStructureOwned = pp -> getStructureList();
	
	if (lStructureOwned.empty() == true) {
		std::cout << "You have no structures man ! You lost then ?\n";
		return;
	}
	
	std::list<UnitPtr> lUnitProduced;
	
	std::list<StructurePtr>::iterator iterS;
	for (iterS = lStructureOwned.begin(); iterS != lStructureOwned.end(); iterS++) {
		StructurePtr sp = (*iterS);
		std::list<UnitPtr> lUnitProducedHere = sp->startTurnProductionUpdate();
		if (lUnitProducedHere.empty() == false) {
			//merging unit lists
			std::cout << "Unit(s) were produced !!!\n";
			lUnitProduced.insert(lUnitProduced.end(), lUnitProducedHere.begin(), lUnitProducedHere.end());
		}
	}
	
	//2. if some were due, we need to add them to player's list
	if (lUnitProduced.empty() == false) {
		pp->addUnitList(lUnitProduced);
	}
	
	//3. if some were due, we need to add them to the correct structure garrison or the correct tile
	//However, this step could be done at an intermediate level in one of the structure's function.
	
	return;
     */
}

void GameEngine::endTurnUpdates(){
	PlayerPtr pp;
	int i = getSpecificPlayer(pp, getActualPlayerTurn());
	
	if (!i) {
		std::cout << "You have a logical problem in your code, dude.\n";
		return;
	}
	
	std::list<StructurePtr> lStructureOwned = pp -> getStructureList();
	
	if (lStructureOwned.empty() == true) {
		std::cout << "You have no structures man !\n";
		return;
	}
	
	std::list<StructurePtr>::iterator iterS;
	for (iterS = lStructureOwned.begin(); iterS != lStructureOwned.end(); iterS++) {
		StructurePtr sp = (*iterS);
		//update Production
		sp -> endTurnProductionUpdate();
		//update Repairing
		
		//update Refueling
	}
	
	pp->incrementTurnNumber();
	
	++actualPlayerIndex;
	
	if (getActualPlayerTurn() == 0) {
		endTurnUpdates();
	}
	
	return;
}

//Actual Main function
int plmain (int argc, char * const argv[]) {
    return 1;
    
    
    // insert code here...
	
	/*
	 * Testing the ObjectFactory...
	 *
	 *
    std::cout << "Welcome to our demonstration of ObjectFactory !\n...\n\n";
	
	HexagonPtr hp( new Hexagon(5, 6));
	
	ObjectParams* objParam = (ObjectParams*) malloc(sizeof(ObjectParams));
	
	objParam -> playerId = 1;
	objParam -> objectId = 2;
	objParam -> hexagon = hp;
	objParam -> type = AIRFIGHTER;
	
	std::cout << "I want an AirFighter...\n";
	ObjectFactory* of = new ObjectFactory();
	
	std::cout << "Factory was initialized...\n";
	UnitPtr u = of->createUnit(objParam);
	
	std::cout << "No more need for factory, deletion...\n";
	free(of);
	free(objParam);
	
	std::cout << "Unit was created :\n";
	int pid = u->getPlayer();
	int uid = u->getId();
	ObjectType type = u->getObjectType();
	int hexX = u->getLocation()->getX();
	int hexY = u->getLocation()->getY();
	
	
	std::cout << "It belongs to player " << pid << ".\n";
	std::cout << "It has an id of " << uid << ".\n";
	std::cout << "It is positioned at x = " << hexX << " and y = "<< hexY << ".\n";
	std::cout << "It has type = " << type << ".\n";
	
	std::cout << "***************************";
	std::cout << "\n-----\nThat's all for ObjectFactory\n-----\n";
	std::cout << "***************************";
	
	
	/*
	 * Testing the ProductionRequest...
	 *
	 *
	
	std::cout << "\n-----\nStarting testing for production requests...\n-----\n";
	
	ProductionRequestPtr requestPtr ( new ProductionRequest(1, AIRFIGHTER, 6) );
	
	std::cout << "Specific constructor...\n";
	int requestId = requestPtr -> getUnitId();
	type = requestPtr -> getObjectType();
	int cost = requestPtr -> getCost();
	
	std::cout << "Request/Unit id : " << requestId << ".\n";
	std::cout << "Type : " << type << ".\n";
	std::cout << "Present cost : " << cost << ".\n";
	
	std::cout << "Let's change some parameters...\n";
	requestPtr -> setUnitId(10234);
	requestPtr -> setObjectType(BOMBER);
	requestPtr -> setCost(10);
	
	requestId = requestPtr -> getUnitId();
	type = requestPtr -> getObjectType();
	cost = requestPtr -> getCost();
	
	std::cout << "Request/Unit id : " << requestId << ".\n";
	std::cout << "Type : " << type << ".\n";
	std::cout << "Present cost : " << cost << ".\n";
	
	std::cout << "Let's update the cost...\n";
	requestPtr -> updateCost();
	
	cost = requestPtr -> getCost();
	std::cout << "New cost : " << cost << ".\n-----\n";
	
	std::cout << "***************************";
	std::cout << "\n-----\nThat's all for ProductionRequest\n-----\n";
	std::cout << "***************************";
	
	
	/*
	 * Testing the Production...
	 *
	 *
	
	std::cout << "\n-----\nStarting testing for Production...\n-----\n";
	
	std::cout << "**0. Constructor...\n";
	
	ProductionPtr p ( new Production(2) );
	std::list<UnitPtr> lUnit;
	int turn = 0;
	
	std::cout << "**0.1 Fields access...\n";
	
	int sid = p -> getStructureId();
	int lrt = p -> getLastRequestTurn();
	int nr = p -> getUnitsInProduction();
	std::string s( p -> getStats() );
	
	std::cout << "\tStructure id : " << sid << ".\n";
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	passProductionTurn(p, lUnit, turn, hp);
	
	std::cout << "**1. Add one request...\n";
	
	p -> addProductionRequest(1354, AIRFIGHTER, 5, turn);
	
	std::cout << "**1.1 Fields access...\n";
	
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	std::cout << "**1.2 Pass turns until request is due...\n";
	
	passProductionTurn(p, lUnit, turn, hp);
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	passProductionTurn(p, lUnit, turn, hp);
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	passProductionTurn(p, lUnit, turn, hp);
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	passProductionTurn(p, lUnit, turn, hp);
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	passProductionTurn(p, lUnit, turn, hp);
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";	
	
	std::cout << "**2. Add more than one productionRequest on the same turn...\n";
	
	std::cout << "First guy requested...\n";
	
	p -> addProductionRequest(1355, TANK, 3, turn);
	
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	std::cout << "Second guy requested...\n";
	
	p -> addProductionRequest(1356, TRANSPORTER, 4, turn);
	
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	std::cout << "End turn...\n";
	
	passProductionTurn(p, lUnit, turn, hp);
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	std::cout << "**3. Add more than one productionRequest on different turns...\n";
	
	std::cout << "Another request...\n";
	
	p -> addProductionRequest(1357, TANK, 3, turn);
	
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	passProductionTurn(p, lUnit, turn, hp);
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	std::cout << "Another request...\n";
	
	p -> addProductionRequest(1358, BOMBER, 2, turn);
	
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	passProductionTurn(p, lUnit, turn, hp);
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	std::cout << "Another request...\n";
	
	p -> addProductionRequest(1359, ARTILLERY, 3, turn);
	
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	passProductionTurn(p, lUnit, turn, hp);
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	passProductionTurn(p, lUnit, turn, hp);
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	std::cout << "**4.Clear production list...\n";
	p -> clearProductionRequests();
	
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	
	std::cout << "Another request...\n";
	
	p -> addProductionRequest(1360, BATTLESHIP, 2, turn);
	
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	passProductionTurn(p, lUnit, turn, hp);
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";
	
	passProductionTurn(p, lUnit, turn, hp);
	lrt = p -> getLastRequestTurn();
	nr = p -> getUnitsInProduction();
	s = p -> getStats();
	std::cout << "\tTurn number for the Last Request : " << lrt << "; Actual turn number :" << turn <<".\n";
	std::cout << "\tNumber of requests : " << nr << ".\n";
	std::cout << "\tUnits in production: \n" << s << ".\n-----\n";	
	
	std::cout << "***************************";
	std::cout << "\n-----\nThat's all for Production\n-----\n";
	std::cout << "***************************";
	
	
	/*
	 * Testing the Structure...
	 *
	 *
	
	std::cout << "\n-----\nStarting testing for Structure...\n-----\n";
	
	//Logic is handled by GameEngine.
	GameEngine* ge = new GameEngine();
	
	if (ge == NULL) {
		std::cout << "We couldn't get any game engine to be created.\n";
		return 0;
	}
	
	//2 Players,
	int numberOfPlayers = 2;
	
	std::cout << "GameEngine is created.\n";
	//instantiation
	ge->createPlayer("Maya"); //an unplayable player - to hold other structures
	ge->createPlayer("PlayerA");
	ge->createPlayer("PlayerB");
	
	std::cout << "Players are created.\n";
	//3 Hexagons, (mimic map creation)
	HexagonPtr hp1( new Hexagon(1,2) );
	HexagonPtr hp2( new Hexagon(3,4) );
	HexagonPtr hp3( new Hexagon(5,6) );
	
	std::cout << "Hexagons are created.\n";
	//3 Structures, (should use the other function with x-y coords)
	ge->createStructure(0, hp1, LAND_CITY);
	ge->createStructure(0, hp2, TOWER); //Starting structure for playerA
	ge->createStructure(0, hp3, TOWER); //Starting structure for playerB
	
	//Why are you feeding player0 as the one who will own those structures?
	//I don't know !
	
	std::cout << "Structures are created.\n";
	//PlayerA acquire his tower from Player0
	ge->acquireStructure(0, 1, 1);
	
	//PlayerB acquire his tower from Player0
	ge->acquireStructure(0, 2, 2);
	
	//Assuming the game is settled !
	std::cout << "The game is settled. Let's start...\n";
	
	//player0 
	ge->endTurnUpdates();
	
	std::cout << "\n****\nLet's try a fake turn ! Now, it is player" << ge->getActualPlayerTurn() << "'s turn.\n****\n";
	
	//player1's turn
	std::cout << "Player" << ge->getActualPlayerTurn() << "'s turn. (should be 1)\n";
	ge->startTurnUpdates();
	ge->endTurnUpdates();
	//player2's turn
	std::cout << "Player" << ge->getActualPlayerTurn() << "'s turn. (should be 2)\n";
	ge->startTurnUpdates();
	ge->endTurnUpdates();
	
	std::cout << "\n****\nFirst Request !\n****\n";
	//player1's turn
	std::cout << "Player" << ge->getActualPlayerTurn() << "'s turn. (should be 1)\n";
	ge->startTurnUpdates();
	
	int i = ge->handleProductionRequest(1, 1, TANK);
	
	if (!i) {
		std::cout << "The request did wrong...\n";
	}else {
		std::cout << "The request did good...\n";
	}
	
	std::cout << "\n****\nTry another request at same structure on the same turn !\n****\n";
	
	i = ge->handleProductionRequest(1, 1, TANK);
	
	if (!i) {
		std::cout << "The request did wrong...\n";
	}else {
		std::cout << "The request did good...\n";
	}
	
	std::cout << "\n****\nTry another request at same structure on the same turn, but different player !\n****\n";
	
	i = ge->handleProductionRequest(2, 1, TANK);
	
	if (!i) {
		std::cout << "The request did wrong...\n\n\n";
	}else {
		std::cout << "The request did good...\n\n\n";
	}
	ge->endTurnUpdates();
	
	//player2's turn
	std::cout << "Player" << ge->getActualPlayerTurn() << "'s turn. (should be 2)\n";
	ge->startTurnUpdates();
	i = ge->handleProductionRequest(1, 1, TANK);
	
	if (!i) {
		std::cout << "The request did wrong...\n\n\n";
	}else {
		std::cout << "The request did good...\n\n\n";
	}
	i = ge->handleProductionRequest(2, 1, AIRFIGHTER);
	
	if (!i) {
		std::cout << "The request did wrong...\n\n\n";
	}else {
		std::cout << "The request did good...\n\n\n";
	}
	i = ge->handleProductionRequest(2, 2, AIRFIGHTER);
	
	if (!i) {
		std::cout << "The request did wrong...\n\n\n";
	}else {
		std::cout << "The request did good...\n\n\n";
	}
	ge->endTurnUpdates();
	
	//player1's turn
	std::cout << "Player" << ge->getActualPlayerTurn() << "'s turn. (should be 1)\n";
	ge->startTurnUpdates();
	i = ge->handleProductionRequest(1, 1, TANK);
	
	if (!i) {
		std::cout << "The request did wrong...\n\n\n";
	}else {
		std::cout << "The request did good...\n\n\n";
	}
	ge->endTurnUpdates();
	
	//player2's turn
	std::cout << "Player" << ge->getActualPlayerTurn() << "'s turn. (should be 2)\n";
	ge->startTurnUpdates();
	ge->endTurnUpdates();
	
	//player1's turn
	std::cout << "Player" << ge->getActualPlayerTurn() << "'s turn. (should be 1)\n";
	ge->startTurnUpdates();
	ge->endTurnUpdates();
	
	//player2's turn
	std::cout << "Player" << ge->getActualPlayerTurn() << "'s turn. (should be 2)\n";
	ge->startTurnUpdates();
	ge->endTurnUpdates();
	
	//player1's turn
	std::cout << "Player" << ge->getActualPlayerTurn() << "'s turn. (should be 1)\n";
	ge->startTurnUpdates();
	ge->endTurnUpdates();
	
	//player2's turn
	std::cout << "Player" << ge->getActualPlayerTurn() << "'s turn. (should be 2)\n";
	ge->startTurnUpdates();
	ge->endTurnUpdates();
	
	//player1's turn
	std::cout << "Player" << ge->getActualPlayerTurn() << "'s turn. (should be 1)\n";
	ge->startTurnUpdates();
	ge->endTurnUpdates();
	
	//player2's turn
	std::cout << "Player" << ge->getActualPlayerTurn() << "'s turn. (should be 2)\n";
	ge->startTurnUpdates();
	ge->endTurnUpdates();
	
	//player1's turn
	std::cout << "Player" << ge->getActualPlayerTurn() << "'s turn. (should be 1)\n";
	ge->startTurnUpdates();
	ge->endTurnUpdates();
	
	//player2's turn
	std::cout << "Player" << ge->getActualPlayerTurn() << "'s turn. (should be 2)\n";
	ge->startTurnUpdates();
	ge->endTurnUpdates();
	
	//Cleaning...
	free(ge);
	
	//END
	std::cout << "End of testing !!!\n";
	
    return 0;
	*/
	
}
