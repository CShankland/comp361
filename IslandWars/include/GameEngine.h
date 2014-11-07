/*
 *  GameEngine.h
 *  IslandWars
 *
 *  Created by Seraphin on 10-03-02.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

/*
 * GameEngine represents the game logic and an interface
 * to the game state.
 *
 * GameEngine creates players, map, and objects as
 * structures when game is loaded. GameEngine answers
 * requests from Graphics and interprets them by answering
 * through other events.
 *
 */


#ifndef _GAME_ENGINE_H_
#define _GAME_ENGINE_H_

#include "Player.h"
#include "Hexagon_Map.h"
#include "ObjectFactory.h"
#include "EventManager.h"
#include "Events.h"
//#include "ToDoList.h" b/c of upadating the path each turn and the way the move is done we had to use a different way
#include "Utils.h"

#include <cstdio>
#include <vector>
#include <map>
#include <list>
#include <deque>

namespace IslandWars {


	//GAMEENGINE

	class GameEngine : public IEventListener
	{

	public:

		//CONSTRUCTORs & DESTRUCTORs
		GameEngine(int nPlayers);
		~GameEngine();

		static GameEngine * get();

		//PLAYER
		void createPlayer();
		void createPlayer(std::string name);

		void setPlayerName(int playerId, std::string name);

		PlayerPtr getActualPlayer();
		void resetActualPlayer();
		const int getGEPlayerId();

		PlayerPtr getSpecificPlayer( int playerId );


		//HEXAGON & MAP
		void initializeMap(int *map);
		
		HexagonPtr getHexagon(int x, int y);
		HexagonPtr getHexagon(Vector2 & position);
		
		bool massEffect( HexagonPtr epicenter, bool (GameEngine::*func)( Vector2 & v ), int radius );
		
		//STRUCTURE & UNIT
		int createStructure(int playerId, int xCoords, int yCoords, ObjectType type);

		UnitPtr generateUnit(int pId, ProductionRequestPtr &prp, int x, int y);  //assist startTurnUpdate to create Unit objects

		//BUILD

		void requestBuild(int structureId, Vector2 v, ObjectType ot);

		// Verification
		int verifyBuild(ObjectType type);

		// Execution
		void build( int structureId, Vector2 v, int unitId, ObjectType ot );
		void handleProductionRequest(ObjectType type, int unitId);

		void cancelBuild( int structureId, Vector2 v, int unitId );
		int cancelProductionRequest(int unitId);

		UnitPtr getUnitPointer(int unitId, Vector2 position);
        bool isStructure( Vector2 & v );

		//SPECIAL ABILITIES
		bool canAct( UnitPtr unitToAct );
		bool isSonar( Vector2& v );
		bool canAttack(UnitPtr defender, UnitPtr attacker);
		void lifeJudgement(UnitPtr unitPtr);
		void battleUnit(UnitPtr defender, UnitPtr attacker);
		void attackSector(HexagonPtr sector, UnitPtr attacker);

		void shell(UnitPtr shelley, UnitPtr sheller);
		bool bomb( Vector2 & groundZero );
		void repair(UnitPtr unitToRepair);
		void refuel(AirUnitPtr unitToRefuel);
		void defuel(AirUnitPtr unitToRefuel);
		void fortify( UnitPtr unitToFortify, bool fortify );
		void conquer( UnitPtr unitConquering, StructurePtr structureToConquer );

		void resetUnit( UnitPtr up );
		void updateFuel( AirUnitPtr up, HexagonPtr hp );

		//MANAGEMENT
		void destroyUnit(UnitPtr unitToDestroy);
		void clearListOfUnits( std::list<UnitPtr> listToDestroy );

		void acquireStructure( PlayerPtr frompp, PlayerPtr topp, StructurePtr sp );
		void acquireStructure( int fromPlayer, int toPlayer, int structureId );


		//SELECTION
		void selectTile(int x, int y);
		void selectStructure();
		int isUnitTypeHere ( HexagonPtr hp, ObjectType typeSought );
		void selectUnit(int uId);
		
		void selectTile(int playerId, int x, int y);
		void selectStructure(int playerId);
		void selectUnit(int playerId, int uId);

		//MOVE

		void requestMove( int unitId, Vector2 from, Vector2 to );
		void unitaryMove( int playerId, int unitId, Vector2 from, Vector2 to, bool isDestination );

		// Verification upon request.
		bool isTerrainMatchingUnit( ObjectType ot, Terrain t );
		bool isOccupiedByEnemies( HexagonPtr hp, int pId );
		bool isOccupiedByAlliedSeaUnit( HexagonPtr hp, int pId );
		bool isOccupiedByTransport( HexagonPtr hp );

		int verifyMovability(bool isDestination);

		// Execution.
		void move( int playerId, int unitId, Vector2 fromCoords, Vector2 toCoords, int mergeCase);
		void landMove( int toX, int toY, int mergeCase );
		void airMove( int toX, int toY, int mergeCase );
		void seaMove( int playerId, int toX, int toY, int mergeCase );

		//TURN UPDATES
		void startTurnUpdates();

		std::list<int> getFreeToBuildStructures();
		bool isFreeToBuildStructure();
		std::list<int> getMovableUnits();
		bool isMovableUnits();
		void executeToDoList();
		void endTurnUpdates();

		int isGameOver( int playerId ); //verify if player still has structures
		void killPlayerUnits( PlayerPtr &pp ); //if player is game over, units are released (maybe faster to change only their state)
		bool isEndGame(); //verify if there is only one player alive, assuming more than one player was playing the game
		
		bool HandleEvent( IEventData & event );

		void printPlayer(int playerId);
		void printPath( std::list< Vector2 > path );

		// Required by the IEventListener.
		char const * GetName( void );
		
	private:

		static const int MAX_PLAYERS = 6;

		int createStructuresOnMap( int * map );

		//PLAYER
		int playerId;

		std::vector<PlayerPtr> playerList; //Note: There will always be a player 0 for unknown structures
		int actualPlayerIndex;

		PlayerPtr actualPlayer;

		//HEXAGON & MAP
		Hexagon_Map * m_HexMap; // This is the main map now

		HexagonPtr selectedTile; //each time user selects
		HexagonPtr lastSelectedTile; //each time user selects, gets selectedTile
		StructurePtr lastSelectedStructure; //if selectedTile has structure on, updated...
		UnitPtr lastSelectedUnit;

		//UNIT & STRUCTURE
		ObjectFactory ofact;

		//TURN UPDATES
		int objectId;
		int totalTurnCompleted;

	};

}

#endif
