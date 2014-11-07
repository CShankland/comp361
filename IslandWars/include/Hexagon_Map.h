/*
 *  Hexagon_Map.h
 *  Boom
 *
 *  Created by Seraphin on 10-02-06.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 
 * (Revision 427)
 * Only the constructor and the getHexagon functions are used directly by Patrick.
 *
 */

#ifndef HEXAGON_MAP_H
#define HEXAGON_MAP_H

#include "Hexagon.h"
#include <boost/shared_ptr.hpp>

#include <vector>
//#include "WillsGlobals.h"
//#include "GraphicsEngine.h"

namespace IslandWars
{

#ifndef DEFAULT_MAP_SIZE
#define DEFAULT_MAP_SIZE 1600
#endif

#ifndef MAX_WIDTH
#define MAX_WIDTH 40
#endif

#ifndef MAX_HEIGHT
#define MAX_HEIGHT 40
#endif
	
	typedef std::vector<HexagonPtr> HexagonVector;
	typedef std::vector<HexagonVector> HexagonMatrix;
	
	class Hexagon_Map {
	public:
		
		//Constructors-Destructors
		Hexagon_Map();
		~Hexagon_Map();
		
		void initialize( int * map );
		
		//Attribute Accessors
		HexagonPtr getHexagon( int x, int y );
		Terrain getTerrainType( int x, int y );
		bool isDefined( int x, int y );
		bool isOccupied( int x, int y );
		
		//Attribute Mutators
		void addUnit( UnitPtr unitPtr, int x, int y );
		void addStructure( StructurePtr unitPtr, int x, int y );
		
		//Debugging functions
		void printHexagon( int x, int y );
		void printOccupancyMap();
		void printTerrainMap();
		
		//bool game_loop2();
	private:
		typedef std::vector< HexagonPtr > HexagonVector;
		typedef std::vector< HexagonVector > HexagonMatrix;
		HexagonMatrix m_Hexagons;		
	};
}
#endif
