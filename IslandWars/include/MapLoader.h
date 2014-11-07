//This header declares the MapLoader class used to load maps into hexArrays.


#ifndef _MAPLOADER_
#define _MAPLOADER_

#include <iostream>
#include "Hexagon_Map.h"

namespace IslandWars {

	class MapLoader{
	public:
		MapLoader(){}
		~MapLoader(){}

		bool loadIWMAP(std::string file, int *hexArray);
		bool loadIWMAP2(int choice, int num, int *hexArray);
	};
}

#endif
