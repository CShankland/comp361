/*
 *  Hexagon.h
 *  Boom
 *
 *  Created by Seraphin on 10-02-06.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *	Last Edited by Guillaume
 *
 *	Now a hexagon holds a structure (potentially) and a list of units (separately) instead of a list of Objects
 *	This will allow for easier ordering of the units on a hexagon and dealing with units inside or outside structures
 *	Adding an object to a hexagon will still work as hexagon will figure dynamcally what kind of object you are adding
 *	and adding an object is overloaded.
 
 *	(Revision 427)
 *  This file is used by Patrick.
 *  All functions except isVisible, isOccupied, and print functions are used directly by Patrick
 *	All private Data Members except visible are used directly by Patrick
 
 *
 */

#pragma once

#ifndef __HEXAGON_H__
#define __HEXAGON_H__


#include "Unit.h"
#include "Structure.h"
#include <list>
#include <iostream>
#include <boost/shared_ptr.hpp>

#ifndef NUM_TERRAINS
#define NUM_TERRAINS 5
#endif


namespace IslandWars
{

	enum Terrain {T_SEA, T_SEA2, T_LAND, T_LAND2, T_MOUNTAIN};
	
    //Guillaume--Replaced the ObjectList by a standard C++ list and all derived operations by appropriate operations
	class Hexagon {
	private:

		StructurePtr hexStructure;
		std::list<UnitPtr> unitList;
		Terrain terrain;
		int xPos;
		int yPos;
		bool visible;


	public:
		Hexagon();
		Hexagon( int x, int y);
		~Hexagon();

		//Attribute Accessors
		int getX();
		int getY();
		Terrain getTerrainType();
		bool isOccupied();
		bool isOccupiedBySeaUnit();
		bool isVisible();
		int size();
		UnitPtr getUnitByPosition(int position);
		UnitPtr getUnitById(int uId);
		StructurePtr getStructure();
		std::list<UnitPtr> getListOfUnits();

		//Attribute Mutators
		void setTerrainType(Terrain t);
		void setVisible(bool isVisible);

		//Operations
		bool compareByHitpoints(UnitPtr unitA, UnitPtr unitB);
		void sortObjects();
		void addObject(StructurePtr structPtr);//For now overload for possible backward compatibility
		void addObject(UnitPtr unitPtr);
		bool remove(UnitPtr unitToRemove);
		void printListOfObjects();
		void printCoordinates();

	};
	
	typedef boost::shared_ptr<Hexagon> HexagonPtr;

}
	
#endif
