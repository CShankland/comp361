/*
 *  TestDriver.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 1/26/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "TestDriver.h"

#include "MapLoader.h"
#include <iostream>
#include "GraphicsEngine.h"
#include "EventManagerImpl.h"
#include "GameEngine.h"
#include "NetworkManager.h"
#include "WillsGlobals.h"
#include "SavingsAndLoads.h"
// Window Title
#define MAIN_TITLE "Island Wars!"

#define MAP_WIDTH 40
#define MAP_HEIGHT 40

using namespace IslandWars;

int Map_Tex_Coords[MAP_WIDTH * MAP_HEIGHT];

int fMain()
{
	num_of_players = 2;
	playerID = 1;
	IEventManager * evtManager = new EventManager( "Event Manager\0", true );
	
	GameEngine * ge = new GameEngine(num_of_players);
	BaseGraphicsEngine * graphics = new BaseGraphicsEngine(ge);
	
	
	int randomNumber;
	srand(time(NULL));
	
	randomNumber = rand() % 5 + 1;
	
	SavingsAndLoads * snl = new SavingsAndLoads(randomNumber);
	
	randomNumber = (loadMode) ? snl->m_MapVal : randomNumber;
	
	MapLoader mpl;
	mpl.loadIWMAP2(mapChoice, randomNumber, Map_Tex_Coords);
	
	GameMap * world = new GameMap;
	world->radius = 2.0;
	world->width = MAP_WIDTH;
	world->height = MAP_HEIGHT;
	world->mapData = &Map_Tex_Coords[0];

	ge->initializeMap(world->mapData);
	if (!loadMode) {
		EndTurnEvent * endTurn = new EndTurnEvent( 0 );
		safeTriggerEvent(*endTurn);
	}
	//ge->game_loop();	
	graphics->loadGame(std::string( MAIN_TITLE ), *world, playerID);
	
	NetworkManager *networkMgr = new NetworkManager();
	networkMgr->Initialize();
	graphics->startGame(loadMode, snl->events);

	return 0;
}
