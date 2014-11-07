/*
 *  ProductionPool.h
 *  ProductionPool
 *
 *  Created by Patrick Desmarais on 10-02-26.
 *  Copyright 2010 McGill University. All rights reserved.
 *
 */

//#include <boost/shared_ptr.hpp>
//#include "Structure.h"
//#include "Unit.h"
//#include "Player.h"
//#include "Production.h"

#ifndef _ProductionPool_
#define _ProductionPool_

namespace IslandWars {

/*
 *	ProductionPool will keep track of units in production.
 *	It will hold a list of Production objects that will
 *	be updated at every end of turn (decrease cost) and 
 *	at every beginning of turn (check for unit to create).
 *	
 *	ProductionPool will interact with UnitFactory to
 *	release units in the game. It will also interact
 *	with Structure at request for production
 *
 */

/*
 *	Should we want to also have a shared pointer with
 *	our list/struct ?
 *
 */

/*Using list instead...
struct PRODUCER;
typedef boost::shared_ptr<PRODUCER> PRODUCERPtr;

struct PRODUCER{
	ProductionPtr pp;
	struct PRODUCER* next;
};
 */
/*
//One-to-one ProductionPool and Player
class ProductionPool {
private:
	PlayerPtr m_pp;
	int prodId; //number and id of Production
	int prodRequestId; //number and id of PROD_REQUEST
	int unitId; //number and id of Units produced
	std::list<ProductionPtr> productionList; //list of Production
	
public:
	ProductionPool();
	ProductionPool(PlayerPtr &pp);
	
	~ProductionPool();
	
	//getters
	const PlayerPtr getPlayerPtr();
	const int getProdId();
	const int getProdRequest();
	const int getUnitId();
	
	const int getCost(ObjectType uName);
	
	//setters
	void setPlayerPtr(PlayerPtr &pp);
	void setProdId(int prodId);
	void setProdRequestId(int prodRequestId);
	void setUnitId(int unitId);
	
	//list functions
	ProductionPtr getProduction(int sId);
	void addProduction(StructurePtr &sp); //creates a Production for a specific Structure
	void deleteProduction(int sId); //deletes a Production according to its Structure reference
	
	//produce unit
	bool handleProductionRequest(int sId, ObjectType uName); //need to feed in the cost, and prodRequestId, unitId
					 //If no production exists at the moment,
					 //ProductionPool will get the structure
					 //to create one.
					 //If production cap is full, it will return
					 //false.
					 //If production request succeeded, it will
					 //return true.

	//cancel production
	//void cancelProductionRequest();
	
	//start turn update
	std::list<UnitPtr> startTurnUpdate(); //returns list of created Unit objects
	
	//end turn update
	void endTurnUpdate(); //iterates through Production objects for endTurnUpdate
};

	typedef boost::shared_ptr<ProductionPool> ProductionPoolPtr;
 */
	
}
#endif