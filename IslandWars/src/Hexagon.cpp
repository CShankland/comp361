/*
 *  Hexagon.h
 *  Boom
 *
 *  Created by Seraphin on 10-02-06.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *	Last Edited by Guillaume
 
 *
 *	(Revision 427)
 *  This file is used by Patrick.
 *  All functions except isVisible, isOccupied, and print functions are used directly by Patrick
 *	All private Data Members except visible are used directly by Patrick
 *
 */


#include "Hexagon.h"
#include <cstdlib>
#include <iostream>

namespace IslandWars
{
	//Constructors and Destructors
	Hexagon::Hexagon() {

		visible = false;
		xPos = 0;
		yPos = 0;

	}

	Hexagon::Hexagon( int x, int y) {

		visible = false;
		xPos = x;
		yPos = y;

	}

	Hexagon::~Hexagon() {
		//printf("Destroying hexagon: %d, %d", xPos, yPos);
		unitList.clear();

	}

	//Attribute Accessors
	int Hexagon::getX() {
		return xPos;
	}

	int Hexagon::getY() {
		return yPos;
	}

	Terrain Hexagon::getTerrainType() {
		return terrain;
	}

	bool Hexagon::isOccupied() {
		if( size() > 0){
			return true;
		}
		return false;
	}
	
	//Returns whether the Hexagon is already occupied by a seaUnit other than a Unit with "ABILITY_SONAR"
	bool Hexagon::isOccupiedBySeaUnit(){
		if( unitList.size() > 0 && ( ( getTerrainType() == T_SEA ) || ( getTerrainType() == T_SEA2 ) ) )
		{
			UnitPtr up;
			int i = 0;
			do{
				up = getUnitByPosition(i);
				i++;
			}while( up && up->getUnitClass() != UC_SEA);

			if( (up.get() != NULL) && !( up->getAbilities() & ABILITY_SONAR ) )
			{
				return true;
			}
		}

		return false;
	}
			

	bool Hexagon::isVisible() {
		return visible;
	}

	int Hexagon::size() {
		return (int) unitList.size();
	}

	UnitPtr Hexagon::getUnitByPosition(int position){

		int i = 0;
		std::list<UnitPtr>::iterator it;
		for( it = unitList.begin(); i != position && i < size(); it++, i++ );
		if ( i >= size() ){
		    Unit * unit = NULL;
		    UnitPtr unitPtr(unit);
            return unitPtr;
		}
		return *it;

	}

	// --------------------------------------------------------
	// Get Unit By Id
	//
	// Checks unitList, and if not found, structureList.
	// --------------------------------------------------------
	UnitPtr Hexagon::getUnitById( int uId ){
		
		std::list<UnitPtr>::iterator iter;
		
		for (iter = unitList.begin(); iter != unitList.end(); ++iter) {
			
			UnitPtr up = (*iter);
			
			if ( up -> getId() == uId ) {
				return up;
			}
			
		}
		
		if (hexStructure.get() != NULL) {
			return hexStructure -> getGarrisonById(uId);
		}
		
		return UnitPtr();
	}
	
	StructurePtr Hexagon::getStructure(){
		return hexStructure;
	}

	//Returns the list of units it holds--often more efficient
	std::list<UnitPtr> Hexagon::getListOfUnits(){
		return Hexagon::unitList;
	}

	//Attribute Mutators

	void Hexagon::setTerrainType(Terrain t) {
		terrain = t;
	}

	void Hexagon::setVisible(bool isVisible) {
		visible = isVisible;
	}

	//Operations
	
	//Needed by sortObjects()
    class HPPredicate{
        public:
            bool operator() ( UnitPtr unitA, UnitPtr unitB ){
                if( unitA->getHitpoints() < unitB->getHitpoints() ){
                    return true;
                }
                return false;
            }
    };

	//Sorts the object putting the ones with less HP at the top (front) of the list
	//Also puts units with ABILITY_HIDE at the bottom (also sorted).-Now works!
	void Hexagon::sortObjects(){

		std::list<UnitPtr>::iterator it;
		int i;

		Hexagon::unitList.sort( HPPredicate() );

		for( i = 0, it = unitList.begin(); ( i < Hexagon::size() ) && ( it != unitList.end() ); i++, it++ ){
			
			UnitPtr currUnit = (*it);

			if( ( currUnit->getAbilities() & ABILITY_HIDE ) ){
				unitList.splice(unitList.end()--, unitList, it);
			}

		}
	}

	//Sets a structure to the Hexagon
	void Hexagon::addObject(StructurePtr structPtr){
		if( !hexStructure ){
			hexStructure = structPtr;
		}
	}

	//Adds a unit to the Hexagon
	void Hexagon::addObject(UnitPtr unitPtr) {

		unitList.push_back(unitPtr);
		sortObjects();

		//std::cout << "changed hexagon" << std::endl;
		this->printCoordinates();
	}

	// --------------------------------------------------------
	// Remove from Hexagon || Structure
	//
	// Returns false if unit was not found !
	// --------------------------------------------------------
	bool Hexagon::remove(UnitPtr unitToRemove){
		
		bool wasFound;
		wasFound = false;
		
		std::list<UnitPtr>::iterator iter;
		UnitPtr potentialUnitToRemove;
		
		for (iter = unitList.begin(); iter != unitList.end(); iter++) {
			potentialUnitToRemove = (*iter);
			
			if ( ( potentialUnitToRemove -> getId() ) == ( unitToRemove -> getId() ) ) {
				wasFound = true;
				iter = unitList.erase(iter);
				break;
			}
			
		}
		
		if ( !wasFound ) {
			// Inside Structure ?
			if (hexStructure.get() != NULL) {
				potentialUnitToRemove = hexStructure -> popGarrison( unitToRemove->getId() );
				
				if ( potentialUnitToRemove.get() != NULL ) {
					wasFound = true;
					
				}
			}
			
		}
		
		return wasFound;
		
	}

	//Debugging functions

	void Hexagon::printListOfObjects() {

		std::list<UnitPtr>::iterator it;
		//std::cout<< std::endl;
		//std::cout << "Objects on Hexagon: ";
		Hexagon::printCoordinates();
		//for(it = unitList.begin(); it != unitList.end(); it++ ){
		//	std::cout<<(*it)->toString()<<"\n"<<std::endl;
		//}
	}

	void Hexagon::printCoordinates() {
		//std::cout << "Coordinate: (" << xPos << ", " << yPos << ")" << std::endl;
	}

}

