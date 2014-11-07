/*
 *  Hexagon_Map.cpp
 *
 *  Created by Guillsumr
 *
 
 * (Revision 427)
 * Only the constructor and the getHexagon functions are used directly by Patrick.
 */


#include "Hexagon_Map.h"

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

	static Hexagon_Map * me;

	//Default Constructor-actually should never be  used
	Hexagon_Map::Hexagon_Map() {
		me = this;
	}
	
	//Used Constructor
	//Initialized with a map
	void Hexagon_Map::initialize(int *map){
		//initializing
		for(int x = 0; x < 40; x++) {
			HexagonVector hexVector;
			for(int y = 0; y < 40; y++) {
				Hexagon * hex = new Hexagon( x, y );
				HexagonPtr hexPtr = HexagonPtr( hex );
				hexVector.push_back(hexPtr);
				int val = map[x * 40 + y];
				if ( val > 4 ){
					if ( val % 10 == 1 ){
						val = 2; //port city on beach
					}else {
						val = 3; //others on land
					}
				}
				//set hexagon's terrain type
				hexVector[y]->setTerrainType( (Terrain) val  );
			}
			m_Hexagons.push_back(hexVector);
		}
	}

	Hexagon_Map::~Hexagon_Map() {
		//printf("\n\n********** Destructing Hexagon Map **********\n\n");
	}

    //Attribute Accessors

	HexagonPtr Hexagon_Map::getHexagon(int x, int y) {
		return m_Hexagons[x][y];
	}

	bool Hexagon_Map::isOccupied( int x, int y) {
		if( x > 39){
			return false;
		}
		if( y > 39){
			return false;
		}
		return ( m_Hexagons[x][y]->isOccupied() );
	}

    Terrain Hexagon_Map::getTerrainType(int x, int y) {
		return m_Hexagons[x][y]->getTerrainType();
	}

	bool Hexagon_Map::isDefined(int x, int y) {
		if (x > 39) return false;
		if (y > 39) return false;

		return true;
	}


	//Adds a unit at the given coordinates
	void Hexagon_Map::addUnit(UnitPtr unitPtr, int x, int y) {
		unitPtr->setLocation(x, y);
		//std::cout << "From HexMap: Changed hexagon "<< m_Hexagons[x][y];
		m_Hexagons[x][y]->addObject(unitPtr);
	}

	//Adds a structure at the given coordinates
	void Hexagon_Map::addStructure(StructurePtr structurePtr, int x, int y) {
		structurePtr->setLocation(x, y);
		//std::cout << "From HexMap: Changed hexagon "<< m_Hexagons[x][y];
		m_Hexagons[x][y]->addObject(structurePtr);
	}

	//Debugging functions

	void Hexagon_Map::printHexagon(int x, int y) {
		HexagonPtr h = m_Hexagons[x][y];

		if(h->isOccupied()) {

			//std::cout << "From HexMap: Hexagon[" << x << "," << y << "] : size = " << h->size() << " : " << h << std::endl;

			//std::cout << "Here we are"<< std::endl;
			h->printListOfObjects();
			//std::cout << "Is ther something wrong?" << std::endl;

		}
	}

	void Hexagon_Map::printOccupancyMap() {

	    for (int y = 0; y < MAX_HEIGHT; y++) {

			for(int x = 0; x < MAX_WIDTH;x++)
				std::cout << isOccupied(x,y) << " ";

			std::cout << std::endl;
		}
	}

	//This function prints the Hexagon Terrain Map
	void Hexagon_Map::printTerrainMap() {

		for (int y = 0; y < MAX_HEIGHT;y++) {

			for(int x = 0; x < MAX_WIDTH;x++)
				std::cout << m_Hexagons[x][y]->getTerrainType() << " ";

			std::cout << std::endl;
		}
	}
}
