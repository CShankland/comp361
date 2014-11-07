/*
 *  Player.cpp
 *
 *  Created by Patrick Desmarais on 10-02-28.
 *  Copyright 2010 McGill University. All rights reserved.
 *

 * (Revision 427)
 * *  This whole file is used by Patrick.
 */

#include "Player.h"

namespace IslandWars
{

	//constructor
	Player::Player(){
		playerName = "Player";
		pId = 0;
		gameOver = 0;

		unitList = std::list<UnitPtr>();
		structureList = std::list<StructurePtr>();
		transportList = std::list<Transport*>();
		toDoMap = std::map<int, std::list<Vector2> >();

		//Initialize statistics to 0
		//Structure
		nLandCity = 0;
		nPortCity = 0;
		nTower = 0;

		//AirUnit
		nAirFighter = 0;
		nBomber = 0;

		//SeaUnit
		nBattleship = 0;
		nCarrier = 0;
		nSubmarine = 0;
		nTransporter = 0;

		//LandUnit
		nArtillery = 0;
		nFlag = 0;
		nTank = 0;
	}

	Player::Player(int pId){
		playerName = "Player";
		this->pId = pId;
		gameOver = 0;

		unitList = std::list<UnitPtr>();
		structureList = std::list<StructurePtr>();
		transportList = std::list<Transport*>();
		toDoMap = std::map<int, std::list<Vector2> >();

		//Initialize statistics to 0
		//Structure
		nLandCity = 0;
		nPortCity = 0;
		nTower = 0;

		//AirUnit
		nAirFighter = 0;
		nBomber = 0;

		//SeaUnit
		nBattleship = 0;
		nCarrier = 0;
		nSubmarine = 0;
		nTransporter = 0;

		//LandUnit
		nArtillery = 0;
		nFlag = 0;
		nTank = 0;
	}

	Player::Player(int pId, std::string name){
		playerName = name;
		this->pId = pId;
		gameOver = 0;

		unitList = std::list<UnitPtr>();
		structureList = std::list<StructurePtr>();
		transportList = std::list<Transport*>();
		toDoMap = std::map<int, std::list<Vector2> >();

		//Initialize statistics to 0
		//Structure
		nLandCity = 0;
		nPortCity = 0;
		nTower = 0;

		//AirUnit
		nAirFighter = 0;
		nBomber = 0;

		//SeaUnit
		nBattleship = 0;
		nCarrier = 0;
		nSubmarine = 0;
		nTransporter = 0;

		//LandUnit
		nArtillery = 0;
		nFlag = 0;
		nTank = 0;
	}

	//destructor
	Player::~Player(){}

	std::string Player::getNToString(){
		char buffer[10];

		std::string retVal("**Object numbers :");

		retVal.append("LandCity ");
		sprintf(buffer, "(%d), ", nLandCity);
		retVal.append(buffer);

		retVal.append("PortCity ");
		sprintf(buffer, "(%d), ", nPortCity);
		retVal.append(buffer);

		retVal.append("Tower ");
		sprintf(buffer, "(%d), ", nTower);
		retVal.append(buffer);

		retVal.append("AirFighter ");
		sprintf(buffer, "(%d), ", nAirFighter);
		retVal.append(buffer);

		retVal.append("Bomber ");
		sprintf(buffer, "(%d), ", nBomber);
		retVal.append(buffer);

		retVal.append("Battleship ");
		sprintf(buffer, "(%d), ", nBattleship);
		retVal.append(buffer);

		retVal.append("Carrier ");
		sprintf(buffer, "(%d), ", nCarrier);
		retVal.append(buffer);

		retVal.append("Submarine ");
		sprintf(buffer, "(%d), ", nSubmarine);
		retVal.append(buffer);

		retVal.append("Transporter ");
		sprintf(buffer, "(%d), ", nTransporter);
		retVal.append(buffer);

		retVal.append("Artillery ");
		sprintf(buffer, "(%d), ", nArtillery);
		retVal.append(buffer);

		retVal.append("Flag ");
		sprintf(buffer, "(%d), ", nFlag);
		retVal.append(buffer);

		retVal.append("Tank ");
		sprintf(buffer, "(%d).\n**\n", nTank);
		retVal.append(buffer);

		return retVal;
	}

	std::string Player::getStats(){

		char buffer[5];

		std::string retVal(" Player #");
		sprintf(buffer, "%d", pId);
		retVal.append(buffer);
		retVal.append("\n- Name : ");
		retVal.append( playerName );
		retVal.append("\n- TurnNumber : ");
		sprintf(buffer, "%d", turnNumber);
		retVal.append(buffer);
		retVal.append("\n- GameOver : ");
		sprintf(buffer, "%d", gameOver);
		retVal.append(buffer);

		retVal.append("\n- Units owned :\n**\n\n");

		std::list<UnitPtr>::iterator iterU;
		for	(iterU = unitList.begin(); iterU != unitList.end(); ++iterU){
			UnitPtr up = (*iterU);
			retVal.append(up->toString());
			retVal.append("\n\n*\n\n");
		}

		retVal.append("\n**\n\n\n- Structures owned :\n");

		std::list<StructurePtr>::iterator iterS;
		for	(iterS = structureList.begin(); iterS != structureList.end(); ++iterS){
			StructurePtr sp = (*iterS);
			retVal.append(sp->getStats());
		}

		return retVal;

	}

	//update number of units upon addition or deletion
	void Player::incrementN(ObjectType ot){
		switch (ot){
				//Structure
			case TOWER:
				++nTower;
				break;

			case LAND_CITY:
				++nLandCity;
				break;

			case PORT_CITY:
				++nPortCity;
				break;

				//AirUnit
			case AIRFIGHTER:
				++nAirFighter;
				break;

			case BOMBER:
				++nBomber;
				break;

				//SeaUnit
			case BATTLESHIP:
				++nBattleship;
				break;

			case TRANSPORTER:
				++nTransporter;
				break;

			case CARRIER:
				++nCarrier;
				break;

			case SUBMARINE:
				++nSubmarine;
				break;

				//LandUnit
			case ARTILLERY:
				++nArtillery;
				break;

			case FLAG:
				++nFlag;
				break;

			case TANK:
				++nTank;
				break;
		}
		return;
	}

	void Player::decrementN(ObjectType ot){
		switch (ot){
				//Structure
			case TOWER:
				--nTower;
				break;

			case LAND_CITY:
				--nLandCity;
				break;

			case PORT_CITY:
				--nPortCity;
				break;

				//AirUnit
			case AIRFIGHTER:
				--nAirFighter;
				break;

			case BOMBER:
				--nBomber;
				break;

				//SeaUnit
			case BATTLESHIP:
				--nBattleship;
				break;

			case TRANSPORTER:
				--nTransporter;
				break;

			case CARRIER:
				--nCarrier;
				break;

			case SUBMARINE:
				--nSubmarine;
				break;

				//LandUnit
			case ARTILLERY:
				--nArtillery;
				break;

			case FLAG:
				--nFlag;
				break;

			case TANK:
				--nTank;
				break;
		}
		return;
	}

	//playerName getter
	const std::string Player::getPlayerName(){
		return playerName;
	}

	//playerName setter
	void Player::setPlayerName(std::string playerName){
		this->playerName = std::string(playerName);
		return;
	}

	//playerId getter
	const int Player::getPlayerId(){
		return pId;
	}

	//playerId setter
	void Player::setPlayerId(int pId){
		this->pId = pId;
		return;
	}

	const int Player::getTurnNumber(){
		return turnNumber;
	}

	void Player::setTurnNumber(int turnNumber){
		this->turnNumber = turnNumber;
		return;
	}

	void Player::incrementTurnNumber(){
		++turnNumber;
		return;
	}

	const int Player::getGameOver(){
		return gameOver;
	}

	void Player::setGameOver(int gameOver){
		this->gameOver = gameOver;
		return;
	}

	//LIST FUNCTIONS

	//get Unit by its unitId
	/* I had some issue with initializing shared_ptr to NULL
	 * while testing, so I decided to put an ObjectPtr as the
	 * argument and return an integer to confirm completion (1 or 0)
	 */

	UnitPtr Player::getUnit(int uId){
		std::list<UnitPtr>::iterator iter;
		for (iter = unitList.begin(); iter != unitList.end(); iter++) {
			UnitPtr upp = (*iter);
			if (upp->getId() == uId){
				return upp;
			}
		}
		return UnitPtr();
	}


	//get Structure by its structureId
	StructurePtr Player::getStructure(int sId){
		std::list<StructurePtr>::iterator iter;
		for (iter = structureList.begin(); iter != structureList.end(); iter++) {
			StructurePtr spp = (*iter);
			if (spp->getId() == sId){
				return spp;
			}
		}
		return StructurePtr();
	}

	//get list of Units
	std::list<UnitPtr>& Player::getUnitList(){
		return unitList;
	}

	std::list<StructurePtr>& Player::getStructureList(){
		return structureList;
	}

	std::list<Vector2>& Player::getPath(int unitId){
	    return toDoMap[unitId];
	}

    void Player::setPath(int unitId, std::list<Vector2>& path){
		toDoMap[unitId] = path;
    }

    void Player::removePath( int unitId ){
		toDoMap.erase(unitId);
		//getPath( unitId ).clear();
    }

	Vector2 Player::pop( int unitId){
		
		Vector2 v;
		
		if (isPath(unitId)) {
			v = toDoMap[unitId].front();
			toDoMap[unitId].pop_front();
		}
		
		return v;
	}
	
	Vector2 Player::front(int unitId){
		Vector2 v = toDoMap[unitId].front();
		return v;
	}
	
	void Player::push_front(int unitId, Vector2 v){
		toDoMap[unitId].push_front(v);
	}
	
	bool Player::isPath(int unitId){
		return (bool)toDoMap.count(unitId);
		
	}
	
	int Player::pathSize(int unitId){
		return (int)toDoMap[unitId].size();
	}
		


	//push Unit to front of list
	void Player::addUnit(UnitPtr up){

		if ((up -> getObjectType()) == TRANSPORTER) {
			//printf("\nTransporter got created for player #%d.\n", pId);
			//std::cout << "Transport List is now : " << transportList.size() << std::endl;
			addTransport(up -> getId());
			//std::cout << "Transport List is now : " << transportList.size() << std::endl;
		}

		//add to unit list
		unitList.push_front(up);

		//increment stats
		incrementN(up->getObjectType());

		return;
	}

	//push Structure to front of list
	void Player::addStructure(StructurePtr sp){

		//add to structure list
		structureList.push_front(sp);

		//increment stats
		incrementN( sp->getObjectType() );

		return;
	}

	//addListOfUnits
	void Player::addUnitList(std::list<UnitPtr> ul){
		//assuming list passed is not empty

		//increment stats
		std::list<UnitPtr>::iterator iter;
		ObjectType ot;

		for (iter = unitList.begin(); iter != unitList.end(); iter++) {
			UnitPtr up = (*iter);
			ot = up -> getObjectType();
			incrementN(ot);
		}

		unitList.insert(unitList.end(), ul.begin(), ul.end());
		return;
	}

	//delete Unit from list
	int Player::deleteUnit(int uId){
		//iterate to find unit
		std::list<UnitPtr>::iterator iter;
		ObjectType ot;

		for (iter = unitList.begin(); iter != unitList.end(); iter++) {
			UnitPtr up = (*iter);
			if (up->getId() == uId){
				//found !

				if ( (up -> getObjectType()) == TRANSPORTER ) {
					std::list<UnitPtr> ul = deleteTransport( up->getId() );

					if ( !ul.empty() ) {
						std::list<UnitPtr>::iterator iterT;

						for (iterT = ul.begin(); iterT != ul.end(); ++iterT) {
							UnitPtr up2 = (*iter);
							deleteUnit( up2 -> getId() );
						}

					}

				}

				//decrement stats
				ot = up -> getObjectType();
				decrementN(ot);

				//erase from structure list
				unitList.erase(iter);

				//successful !
				return 1;
			}
		}
		//not found!
		return 0;
	}

	//delete Structure from list
	int Player::deleteStructure(int sId){
		//iterate to find structure
		std::list<StructurePtr>::iterator iter;
		ObjectType ot;

		for (iter = structureList.begin(); iter != structureList.end(); iter++) {
			StructurePtr sp = (*iter);
			if (sp->getId() == sId){
				//found!

				//decrement stats
				ot = sp -> getObjectType();
				decrementN(ot);

				//erase from structure list
				structureList.erase(iter);

				//successful !
				return 1;
			}
		}
		//not found!
		return 0;
	}

	//TRANSPORT

	// --------------------------------------------------------
	// Add Transporter to TransporterList.
	// --------------------------------------------------------
	void Player::addTransport(int transportId){

		Transport* t = new Transport(transportId);
		transportList.push_back(t);
		return;

	}

	// --------------------------------------------------------
	// Delete Transporter from TransporterList and all
	// transported Units.
	// --------------------------------------------------------
	std::list<UnitPtr> Player::deleteTransport(int transportId){

		std::list<UnitPtr> carryCopy;

		if (transportList.empty()) {
			return carryCopy;
		}

		std::list<Transport*>::iterator iter;

		for (iter = transportList.begin(); iter != transportList.end(); ++iter) {
			Transport* t = (*iter);

			if ( (t -> transportId) == transportId) {
				carryCopy = (t->carry);
				(t -> carry).clear();

				transportList.erase(iter);

				return carryCopy;
			}

		}

		return carryCopy;

	}

	// --------------------------------------------------------
	// Get transported Unit in specific Transporter.
	// --------------------------------------------------------
	UnitPtr Player::getTransportedUnit(int transportId, int unitId){

		if (transportList.empty()) {
			return UnitPtr();
		}

		std::list<Transport*>::iterator iter;

		for (iter = transportList.begin(); iter != transportList.end(); ++iter) {
			Transport* t = (*iter);

			if ( (t -> transportId) == transportId) {
				std::list<UnitPtr>::iterator iterCarry;
				for (iterCarry = (t -> carry).begin(); iterCarry != (t -> carry).end(); ++iterCarry) {
					UnitPtr up = (*iterCarry);
					if (up -> getId() == unitId) {
						return up;
					}
				}

				break;
			}

		}

		return UnitPtr();

	}

	// --------------------------------------------------------
	// Get List of transported Units in specific Transporter.
	// --------------------------------------------------------
	std::list<UnitPtr> Player::getTransportedUnitList( int transportId ){
		
		std::list<UnitPtr> ul;
		
		std::list<Transport*>::iterator iter;
		
		for (iter = transportList.begin(); iter != transportList.end(); ++iter) {
			Transport* t = (*iter);
			
			if ( (t -> transportId) == transportId) {
				return (t -> carry);
			}
			
		}
		
		return ul;
	}
	
	// --------------------------------------------------------
	// Add transported Unit in specific Transporter.
	// --------------------------------------------------------
	void Player::addTransportedUnit(int transportId, UnitPtr up){

		//std::cout << "Adding transported unit to transport: " << transportId << " TransportList size: " << transportList.size() << std::endl;
		
		if (transportList.empty()) {
			return;
		}

		std::list<Transport*>::iterator iter;

		for (iter = transportList.begin(); iter != transportList.end(); ++iter) {
			//Transport* t = (*iter);

			if ( ((*iter) -> transportId) == transportId) {
				((*iter) -> carry).push_back( up );
				//std::cout << "Added transported unit." << std::endl;
				break;
			}

		}

		return;
	}

	// --------------------------------------------------------
	// Remove transported Unit in specific Transporter.
	// --------------------------------------------------------
	UnitPtr Player::removeTransportedUnit(int transportId, int unitId){

		if (transportList.empty()) {
			return UnitPtr();
		}

		std::list<Transport*>::iterator iter;

		for (iter = transportList.begin(); iter != transportList.end(); ++iter) {
			Transport* t = (*iter);

			if ( (t -> transportId) == transportId) {
				std::list<UnitPtr>::iterator iterCarry;
				for (iterCarry = (t -> carry).begin(); iterCarry != (t -> carry).end(); ++iterCarry) {
					UnitPtr up = (*iterCarry);
					if (up -> getId() == unitId) {
						(t -> carry).erase(iterCarry);
						return up;
					}
				}

				break;
			}

		}

		return UnitPtr();

	}

	// --------------------------------------------------------
	// Clear Transporter from any Units.
	// --------------------------------------------------------
	std::list<UnitPtr> Player::clearTransport(int transportId){

		std::list<UnitPtr> carryCopy;

		if (transportList.empty()) {
			return carryCopy;
		}

		std::list<Transport*>::iterator iter;

		for (iter = transportList.begin(); iter != transportList.end(); ++iter) {
			Transport* t = (*iter);

			if ( (t -> transportId) == transportId) {
				carryCopy = (t->carry);
				(t -> carry).clear();

				return carryCopy;
			}

		}

		return carryCopy;

	}

	//STATISTICS
	//Structure
	const int Player::getNLandCity(){
		return nLandCity;
	}
	//void setNLandCity(int n)

	const int Player::getNPortCity(){
		return nPortCity;
	}
	//void setNPortCity(int n);

	const int Player::getNTower(){
		return nTower;
	}
	//void setNTower(int n);

	//AirUnit
	const int Player::getNAirFighter(){
		return nAirFighter;
	}
	//void setNAirFighter(int n);

	const int Player::getNBomber(){
		return nBomber;
	}
	//void setNBombers(int n);

	//SeaUnit
	const int Player::getNBattleship(){
		return nBattleship;
	}
	//void setNBattleship(int n);

	const int Player::getNCarrier(){
		return nCarrier;
	}
	//void setNCarrier(int n);

	const int Player::getNSubmarine(){
		return nSubmarine;
	}
	//void setNSubmarine(int n);

	const int Player::getNTransporter(){
		return nTransporter;
	}
	//void setNTransporter(int n);

	//LandUnit
	const int Player::getNArtillery(){
		return nArtillery;
	}
	//void setNArtillery(int n);

	const int Player::getNFlag(){
		return nFlag;
	}
	//void setNFlag();

	const int Player::getNTank(){
		return nTank;
	}
	//void setNTank(int n);

	//Debugging functions

	void Player::printListOfUnits(){

		std::list<UnitPtr>::iterator it;
		std::cout<< std::endl;
		std::cout << "Units of Player: "<<Player::getPlayerName()<<"\n";
		for(it = unitList.begin(); it != unitList.end(); it++ ){
			std::cout<<(*it)->toString()<<"\n"<<std::endl;
		}

	}
}
