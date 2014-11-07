/*
 *  Production.cpp
 *
 *  Created by Patrick Desmarais on 10-02-26.
 *  Copyright 2010 McGill University. All rights reserved.
 *
 
 * (Revision 427) 
 *  This whole file is used by Patrick.
 *
 */

#include "Production.h"
#include <stdlib.h>
#include <string>

namespace IslandWars
{
	std::string Production::getTypeToString(ObjectType objectType){
		switch (objectType){
				//Structure
			case TOWER:
				return std::string("Tower");
				
			case LAND_CITY:
				return std::string("Land City");
				
			case PORT_CITY:
				return std::string("Port City");
				
				//AirUnit
			case AIRFIGHTER:
				return std::string("Air Fighter");
				
			case BOMBER:
				return std::string("Bomber");
				
				//SeaUnit
			case BATTLESHIP:
				return std::string("Battleship");
				
			case TRANSPORTER:
				return std::string("Transporter");
				
			case CARRIER:
				return std::string("Carrier");
				
			case SUBMARINE:
				return std::string("Submarine");
				
				//LandUnit
			case ARTILLERY:
				return std::string("Artillery");
				
			case FLAG:
				return std::string("Flag");
				
			case TANK:
				return std::string("Tank");
			default :
				return std::string("Undefined Unit");
		}
	}
	
	//finds the next unit to create
	//=> returns 1 if there is one, 0 otherwise
	//"returns" next cost-0 ProductionRequestPtr in prp
	int Production::popNextProductionRequestDue(ProductionRequestPtr &prp){ //returns number of units to be produced
		
		std::list<ProductionRequestPtr>::iterator iter;
		for (iter=productionRequestList.begin(); iter != productionRequestList.end(); ++iter) {
			ProductionRequestPtr productionRequestPtr = (*iter);
			if (productionRequestPtr->getCost() <= 0) {
				prp = productionRequestPtr;
				iter = productionRequestList.erase(iter);//deleteProductionRequest(iter, 1);
				return 1;
			}
		}
		
		return 0;
	}
	
	//Constructors
	Production::Production(){
		structureId = -1;
		lastRequestTurn = -1;
		
		productionRequestList = std::list<ProductionRequestPtr>();
	}
	
	Production::Production(int structureId){
		this->structureId = structureId;
		lastRequestTurn = -1;
		
		productionRequestList = std::list<ProductionRequestPtr>();
	}
	
	//Destructor
	Production::~Production(){}
	
	//Getters
	const int Production::getStructureId(){
		return structureId;
	}
	
	const int Production::getLastRequestTurn(){
		return lastRequestTurn;
	}
	
	const int Production::getUnitsInProduction(){
		return (int) productionRequestList.size();
	}
	
	//Get specific production in terms of type and cost
	std::list<ProductionRequestPtr>::iterator Production::getProductionRequest(ObjectType objectType, int cost){
		std::list<ProductionRequestPtr>::iterator iter;
		for (iter=productionRequestList.begin(); iter != productionRequestList.end(); ++iter) {
			ProductionRequestPtr productionRequestPtr = (*iter);
			if (productionRequestPtr->getCost() == cost && productionRequestPtr->getObjectType() == objectType) {
				return iter;
			}
		}
	 	iter = productionRequestList.end();
		++iter;
		
		return iter;
	}
	
	ProductionRequestPtr Production::getProductionRequest(int unitId){
		std::list<ProductionRequestPtr>::iterator iter;
		for (iter=productionRequestList.begin(); iter != productionRequestList.end(); ++iter) {
			ProductionRequestPtr productionRequestPtr = (*iter);
			if (productionRequestPtr->getUnitId() == unitId) {
				return productionRequestPtr;
			}
		}
		
		return ProductionRequestPtr();
	}
	
	std::string Production::getStats(){ //returns string to display in statsWindow
		std::string statsStr("");
		char c[8];
		
		std::list<ProductionRequestPtr>::iterator iter;
		for (iter = productionRequestList.begin(); iter != productionRequestList.end(); iter++) {
			ProductionRequestPtr productionRequestPtr = (*iter);
			
			statsStr.append(getTypeToString(productionRequestPtr->getObjectType()));
			statsStr += " (";
			sprintf(&c[0], "%d", productionRequestPtr->getCost()); //trying to change integer to string
			statsStr.append(c);
			statsStr += " turns left);";
		}
		return statsStr;
	}
	
	
	
	//Setters
	void Production::setStructureId(int structureId){
		this->structureId = structureId;
		return;
	}
	
	void Production::setLastRequestTurn(int a_lastRequestTurn)
	{
		lastRequestTurn = a_lastRequestTurn;
	}
	
	//REQUESTS
	
	//ADDITION
	
	// Verification
	
	// --------------------------------------------------------
	// Is Production Full for this turn ?
	// --------------------------------------------------------
	bool Production::isProductionFull(int currentTurn){

		if (lastRequestTurn == currentTurn) {
			return true;
		}
		
		return false;
	}
	
	
	// Execution
	
	// --------------------------------------------------------
	// Add Production Request
	// --------------------------------------------------------
	void Production::addProductionRequest(int unitId, ObjectType objectType, int cost, int currentTurn)
	{
		
		ProductionRequestPtr productionRequestPtr( new ProductionRequest(unitId, objectType, cost) );
		
		this->productionRequestList.push_back(productionRequestPtr);
		this->lastRequestTurn = currentTurn;
		
		return;
	}
	
	
	
	//once unit is produced, we can delete the request.
	void Production::deleteProductionRequest(std::list<ProductionRequestPtr>::iterator &iter, int forProduction)
	{
		if (forProduction == 0) {
			lastRequestTurn--; 
		}
		productionRequestList.erase(iter);
		return;
	}
	
	void Production::deleteProductionRequest(int unitId, int forProduction)
	{
		if (forProduction == 0) {
			lastRequestTurn--;
		}
		std::list<ProductionRequestPtr>::iterator iter;
		for (iter=productionRequestList.begin(); iter != productionRequestList.end(); ++iter) {
			ProductionRequestPtr productionRequestPtr = (*iter);
			if ((productionRequestPtr -> getUnitId()) == unitId) {
				iter = productionRequestList.erase(iter);
				break;
			}
		}
		return;
	}
	
	void Production::clearProductionRequests(){
		
		productionRequestList.clear();
		
		return;
	}
	
	//Cycle through every units to update their cost and "check" for units to produce next turn
	int Production::endTurnUpdate(){ //update cost down
		int unitsToProduceNextTurn = 0;
		
		//iterate through list
		std::list<ProductionRequestPtr>::iterator iter;
		for (iter=productionRequestList.begin(); iter != productionRequestList.end(); ++iter) {
			
			ProductionRequestPtr productionRequestPtr = (*iter);
			
			if(productionRequestPtr->updateCost() == 1){
				unitsToProduceNextTurn = 1;
			}
		}
		return unitsToProduceNextTurn;
	}
	
	//verify if any ProductionRequest is due, producing Unit if any are due
	std::list<ProductionRequestPtr> Production::startTurnUpdate(){
		
		ProductionRequestPtr productionRequestPtr;
		int i = popNextProductionRequestDue(productionRequestPtr);
		
		std::list<ProductionRequestPtr> productionRequestPtrList;
		
		while(i != 0){
			productionRequestPtrList.push_back(productionRequestPtr);
			i = popNextProductionRequestDue(productionRequestPtr);
		}
		
		return productionRequestPtrList;
	}
}