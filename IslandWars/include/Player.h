/*
 *  Player.h
 *  IslandWarsHome
 *
 *  Created by Patrick Desmarais on 10-02-28.
 *  Copyright 2010 McGill University. All rights reserved.
 *

 * (Revision 427)
 * *  This whole file is used by Patrick.
 */

/*
 *	Discussion on performance:
 *	To increase performance, we could limit the number of units.
 *	The number of structures is already limited depending on the
 *	map.
 *	Hence, a hash map could get us the right unit/structure
 *	very rapidly.
 */

#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Unit.h"
#include "Structure.h"
#include "Utils.h"
#include <string>
#include <list>
#include <map>
#include <boost/shared_ptr.hpp>
#include <cstdlib>

namespace IslandWars {

	//class to manage transportation of Units through Transporter
	class Transport {
	public:
		int transportId;
		std::list<UnitPtr> carry;

		Transport(){
			carry = std::list<UnitPtr>();
			transportId = 0;
		}

		Transport( int transportId ){
			carry = std::list<UnitPtr>();
			this -> transportId = transportId;
		}

		~Transport(){}
	};

	//shared_ptr for Player
	class Player;
	typedef boost::shared_ptr<Player> PlayerPtr;

	/*
	 *	Player class will keep track of its id,
	 *	the units and structures of the player for updates to
	 *	structures (production requests cost, repairing, refueling)
	 *	and to units (carrier refueling, units' remaining moves)
	 *	and if it's the player's turn.
	 *
	 */

	class Player {
	private:
		std::string playerName;
		int pId;
		int turnNumber;
		int gameOver; //if he has lost all towers

		std::list<UnitPtr> unitList;
		std::list<StructurePtr> structureList;
		std::map<int, std::list<Vector2> > toDoMap;

		std::list<Transport*> transportList;

		std::string getNToString();

		//STATISTICS
		//Structure
		int nLandCity;
		int nPortCity;
		int nTower;

		//AirUnit
		int nAirFighter;
		int nBomber;

		//SeaUnit
		int nBattleship;
		int nCarrier;
		int nSubmarine;
		int nTransporter;

		//LandUnit
		int nArtillery;
		int nFlag;
		int nTank;

		//update number of units upon addition or deletion
		void incrementN(ObjectType ot);
		void decrementN(ObjectType ot);

	public:
		// constructor
		Player();
		Player(int pId);
		Player(int pId, std::string name);

		// destructor
		~Player();

		//Printer
		std::string getStats();

		// playerName.
		const std::string getPlayerName();
		void setPlayerName(std::string playerName);

		// playerId.
		const int getPlayerId();
		void setPlayerId(int pId);

		// turnNumber.
		const int getTurnNumber();
		void setTurnNumber(int turnNumber);
		void incrementTurnNumber();

		// gameOver.
		const int getGameOver();
		void setGameOver(int gameOver);
		
		//PATHS
        std::list<Vector2>& getPath(int unitId);
        void setPath(int unitId, std::list<Vector2>& path);
        void removePath(int unitId);
        Vector2 pop(int unitId);
		Vector2 front(int unitId);		
		void push_front(int unitId, Vector2 v);		
		bool isPath(int unitId);
		int pathSize(int unitId);
		
		//LISTS
		//Accessors
		UnitPtr getUnit(int uId);
		StructurePtr getStructure(int sId);
		
		std::list<UnitPtr>& getUnitList();
		std::list<StructurePtr>& getStructureList();
		
		//Adders
		void addUnit(UnitPtr up);
		void addStructure(StructurePtr sp);

		void addUnitList(std::list<UnitPtr> ul);

		//Removers (0 : failed, 1: sucess).
		int deleteUnit(int uId);
		int deleteStructure(int sId);

		//TRANSPORT
		void addTransport(int transportId);
		std::list<UnitPtr> deleteTransport(int transportId);

		UnitPtr getTransportedUnit(int transportId, int unitId);
		std::list<UnitPtr> getTransportedUnitList( int transportId );
		
		void addTransportedUnit(int transportId, UnitPtr up);
		UnitPtr removeTransportedUnit(int transportId, int unitId);
		std::list<UnitPtr> clearTransport(int transportId);

		//STATISTICS
		//Structure
		const int getNLandCity();
		const int getNPortCity();
		const int getNTower();

		//AirUnit
		const int getNAirFighter();
		const int getNBomber();

		//SeaUnit
		const int getNBattleship();
		const int getNCarrier();
		const int getNSubmarine();
		const int getNTransporter();
		
		//LandUnit
		const int getNArtillery();
		const int getNFlag();
		const int getNTank();

		void printListOfUnits();



	};
}
#endif
