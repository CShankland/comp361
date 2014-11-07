/*
 *  ProductionPool.cpp
 *  ProductionPool
 *
 *  Created by Patrick Desmarais on 10-02-26.
 *  Copyright 2010 McGill University. All rights reserved.
 *
 */

#include "ProductionPool.h"

//DATA MEMBERS
//
//PlayerPtr pp;
//int prodId; //number and id of Production
//int prodRequestId; //number and id of PROD_REQUEST
//int unitId; //number and id of Units produced
//list<ProductionPtr> productionList; //list of Production
//

namespace IslandWars
{
	/*
	ProductionPool::ProductionPool()
	{
		//Empty constructor
		m_pp = PlayerPtr();
		prodId = 0;
		prodRequestId = 0;
		unitId = 0;
	}
	
	ProductionPool::ProductionPool(PlayerPtr &pp){
		m_pp = pp;
		prodId = 0;
		prodRequestId = 0;
		unitId = 0;
	}
	
	ProductionPool::~ProductionPool(){
		//delete pointer to Player
		m_pp.~shared_ptr();
		
		//delete list
		std::list<ProductionPtr>::iterator iter;
		for (iter = productionList.begin(); iter != productionList.end(); iter++) {
			ProductionPtr pptr = (*iter);
			pptr->~Production();
			pptr.~shared_ptr();
		}
		productionList.~list();
	}
	
	//getters
	const PlayerPtr ProductionPool::getPlayerPtr(){
		return m_pp;
	}
	const int ProductionPool::getProdId(){
		return prodId;
	}
	
	const int ProductionPool::getProdRequest(){
		return prodRequestId;
	}
	
	const int ProductionPool::getUnitId(){
		return unitId;
	}
	
	
	const int ProductionPool::getCost(Object_Type uName){
		switch (uName) {
			case TANK:
				return 2;
			case ARTILLERY:
				return 3;
			case FLAG:
				return 2;
			case BATTLESHIP:
				return 5;
			case SUBMARINE:
				return 4;
			case TRANSPORT:
				return 3;
			case AIRFIGHTER:
				return 3;
			default:
				return 3;
		}
	}
	
	//setters
	void ProductionPool::setPlayerPtr(PlayerPtr &pp){
		m_pp = pp;
		return;
	}
	void ProductionPool::setProdId(int prodId){
		this->prodId = prodId;
		return;
	}
	void ProductionPool::setProdRequestId(int prodRequestId){
		this->prodRequestId = prodRequestId;
		return;
	}
	void ProductionPool::setUnitId(int unitId){
		this->unitId = unitId;
		return;
	}
	
	//finds or not the right production
	ProductionPtr ProductionPool::getProduction(int sId)
	{
		//look for right Production
		std::list<ProductionPtr>::iterator iter;
		for (iter = productionList.begin(); iter != productionList.end(); iter++) {
			ProductionPtr pptr = (*iter);
			StructurePtr sp = pptr->getStructurePtr();
			if (sp->getId() == sId){
				//found!
				return pptr;
			}
		}
		//!found
		//ProductionPtr pptr();
		return ProductionPtr();
	}
	
	//add Production object for specific Structure object
	void ProductionPool::addProduction(StructurePtr &sp) //creates a Production for a specific Structure
	{
		//instantiation
		ProductionPtr pptr( new Production(prodId, sp) );
		prodId++;
		
		//addition
		productionList.push_back(pptr);
		
		//Structure update
		//sp->setProduction(pptr); -- Will - Compiler complaining aboutno setProd method
		
		return;
	}
	
	//retrieves and delete production
	void ProductionPool::deleteProduction(int sId) //deletes a Production according to its Structure reference
	{
		ProductionPtr pptr = getProduction(sId);
		if (pptr.get() == NULL) {
			//not found!
			return;
			
		}else {
			//delete Production
			pptr->~Production();
			pptr.~shared_ptr();
			
			return;
		}
	}
	
	
	//produce unit
	bool ProductionPool::handleProductionRequest(int sId, Name uName) //need to feed in the cost, and prodRequestId
	{
		//If no production exist at the moment,
		//ProductionPool will get the structure
		//to create one.
		ProductionPtr pptr = getProduction(sId);
		if (pptr.get() == NULL) {
			StructurePtr sp = m_pp->getStructure(sId);
			if (sp.get() == NULL) {
				return false;
			}else {
				addProduction(sp);
			}
		}
		
		
		//If production cap is full, it will return
		//false.
		if (pptr->getUnitsInProduction() >= 4) { //now limited to 4 unitsMax
			return false;
		}else {
			//calculating cost depending on unit type
			int cost = getCost(uName);
			
			//new ProductionRequest...
			pptr->addProductionRequest(prodRequestId, unitId, uName, cost);
			prodRequestId++;
			unitId++;
		}
		
		
		//If production request succeeded, it will
		return true;
		
	}
	
	//cancel production
	//void cancelProductionRequest();
	
	//start turn update
	std::list<UnitPtr> ProductionPool::startTurnUpdate() //returns list of created Unit objects
	{
		//store lists of units produced
		std::list<UnitPtr> unitList;
		
		//iterate through each Production to update them
		std::list<ProductionPtr>::iterator iter;
		
		//each Production adds his contribution to unitList
		for (iter = productionList.begin(); iter != productionList.end(); iter++) {
			ProductionPtr pptr = (*iter);
			
			//get list of units produced for this Production
			std::list<UnitPtr> unitSubList = pptr->startTurnUpdate();
			
			//if list of this Production is not empty, add it to unitList
			if (unitSubList.empty() == false) {
				
				std::list<UnitPtr>::iterator iter2;
				for (iter2 = unitSubList.begin(); iter2 != unitList.end(); iter2++) {
					UnitPtr up = (*iter2);
					unitList.push_back(up);
				}
				
			}
		}
		
		return unitList;
	}
	
	//end turn update
	void ProductionPool::endTurnUpdate() //iterates through Production objects for endTurnUpdate
	{
		//iterate through each Production to update them
		std::list<ProductionPtr>::iterator iter;
		//each Production adds his contribution to unitList
		for (iter = productionList.begin(); iter != productionList.end(); iter++) {
			ProductionPtr pptr = (*iter);
			pptr->endTurnUpdate();
		}
		
		return;
	}
	 */
}