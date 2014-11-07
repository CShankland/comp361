/*
 *  WillsGlobals.h
 *  IslandWars
 *
 *  Created by Seraphin on 10-03-03.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#pragma once
#ifndef _WILLS_GLOBALS_
#define _WILLS_GLOBALS_
#include "network.h"
#include <string>


struct StartUpInfo {
	bool isServer;
	bool isLoad;
	
	char theName;
	int num_of_players;
	int num_of_players_left;
};


class GameMap {
public:
	double radius;
	int width;
	int height;
	int * mapData;
};

extern int playerID;
extern int num_of_players;

extern bool isServer;
extern bool loadMode;
extern bool userServer;

extern int mapChoice;

extern std::string filename;

extern std::string basePath;

#endif