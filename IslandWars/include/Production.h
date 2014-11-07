/*
 *  Production.h
 *
 *  Created by Patrick Desmarais on 10-02-26.
 *  Copyright 2010 McGill University. All rights reserved.
 *
 
 * (Revision 427) 
 *  This whole file is used by Patrick.
 *
 */


#pragma once
#ifndef PRODUCTION_H_
#define PRODUCTION_H_

#include <string>
#include <list>
#include <boost/shared_ptr.hpp>
#include "Object.h"
#include "ProductionRequest.h"

namespace IslandWars
{
	
	//One-to-one Production object to Structure object
	class Production {
	private:
		int structureId;
		int lastRequestTurn; //to allow only one request per turn
		std::list<ProductionRequestPtr> productionRequestList;
		
		//private functions
		std::string getTypeToString(ObjectType objectType);
		
		//finds the next unit to create
		int popNextProductionRequestDue(ProductionRequestPtr &prp); //returns next cost-0 ProductionRequestPtr in prp
		
	public:
		Production();
		Production(int structureId);
		~Production();
		
		//getters
		const int getStructureId();
		const int getLastRequestTurn();
		const int getUnitsInProduction(); //number of productionRequest
		std::list<ProductionRequestPtr> getPendingProductions() { return productionRequestList; }
		//Get specific production in terms of type and cost
		std::list<ProductionRequestPtr>::iterator getProductionRequest(ObjectType uName, int cost);
		ProductionRequestPtr getProductionRequest(int unitId);
		
		//triggered from Structure
		std::string getStats(); //requests string to display in statsWindow
		
		//setters
		void setStructureId(int structureId);
		void setLastRequestTurn(int lastRequestTurn);
		
		//REQUESTS
		
		//ADDITION
		
		// Verification		
		bool isProductionFull(int currentTurn);
		
		// Execution
		void addProductionRequest(int unitId, ObjectType objectType, int cost, int currentTurn);
		
		//Once unit is produced, we can delete the request.
		void deleteProductionRequest(std::list<ProductionRequestPtr>::iterator &iter, int forProduction);
		void deleteProductionRequest(int unitId, int forProduction);
		void clearProductionRequests(); //when structure is lost by the player
		
		int endTurnUpdate();//Cycle through every units to update their cost and return the "check" for units to produce next turn
		
		std::list<ProductionRequestPtr> startTurnUpdate(); //verify if any ProductionRequest is due
	};
	typedef boost::shared_ptr<Production> ProductionPtr;
	
}

#endif
