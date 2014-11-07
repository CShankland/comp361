
/* A header file containing information for all structures.
 Author: Guillaume Viger
 Last Edited: 23/02/10
 
 * (Revision 427) 
 *  This whole file is used by Patrick.
 *
 */

#pragma once

#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "Object.h"
#include "Production.h"
#include "Unit.h"
#include <list>
#include <boost/shared_ptr.hpp>


namespace IslandWars
{
	typedef std::list<ObjectType> ProductionCapabilities;
	
	class Structure : public Object {
    public:
		Structure( int playerId, int objectId, int x, int y, ObjectType objectType, int radarRange, ProductionCapabilities prodCaps );
        Structure( ObjectParams * objectParams, int radarRange, ProductionCapabilities prodCaps );
		
		int getRadarRange();
		ProductionCapabilities getProductionCapabilities();
		
		//**** Production Public Member Functions ****
		
		//Get costs of different units
		const int getCost(ObjectType objectType);
		
		Production getProduction();
		int getProductionIndicator();
		void setProductionIndicator(int productionIndicator);
		
		//BUILD
		//ADDITION
		
		// Verification
		int verifyBuild(ObjectType objectType, int actualTurn);
		
		// Execution
		void handleProductionRequest(int unitId, ObjectType objectType, int actualTurn);
		
		//cancel production
		//returns 1 if successful, 0 if not...
		int cancelProductionRequest(int unitId);
		
		//clear ProductionRequest list of Production p
		void clearProduction();
		
		//production update at start of turn
		std::list<ProductionRequestPtr> startTurnProductionUpdate(); //returns list of created objects
		
		//production update at end of turn
		bool isFreeToBuild( int actualTurn );
		void endTurnProductionUpdate(); //iterates through Production objects for endTurnUpdate
		
		
		
		//**** Structure Public Member Functions ****
		
		int garrisonSize();
		
		//push unit in Garrison
		void pushGarrison( UnitPtr &up );
		
		//get top unit from garrison
		UnitPtr topGarrison(); //undestructive access
		UnitPtr popGarrison(); //destructve access
		
		UnitPtr getGarrisonByPosition( int position );
		
		//get specific unit from garrison
		UnitPtr getGarrisonById( int uId ); //undestructive
		UnitPtr popGarrison( int uId ); //destructive
		
		//clear garrison ; interesting for bomber
		std::list<UnitPtr> clearGarrison();
		
		
		//**** Other Public Member Functions ****
		
		bool isUnderAttack();
		void setUnderAttack( bool underAttack );

		std::string getStats();
		
		
	private:
		int m_RadarRange;
		ProductionCapabilities m_ProdCaps;
		
		Production production;
		int productionIndicator;
		
		std::list<UnitPtr> garrison;
		bool underAttack;
		bool repairedThisTurn;
		
		//verify if request is in m_ProdCaps
		//1 = possible; 0 = not possible
		bool isProductionPossible(ObjectType objectType);
		
		//private functions
		std::string getTypeToString(ObjectType objectType);
		
		
		std::string radarRangeToString();
		std::string productionCapabilitiesToString();
		std::string productionToString();
	};
	typedef boost::shared_ptr<Structure> StructurePtr;
}
#endif
