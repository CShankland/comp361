/*
 *  ProductionRequest.cpp
 *  IslandWarsHome
 *
 *  Created by Patrick Desmarais on 10-03-01.
 *  Copyright 2010 McGill University. All rights reserved.
 *
 * (Revision 427) 
 *  This whole file is used by Patrick.
 *
 */

#include "ProductionRequest.h"

namespace IslandWars {

	ProductionRequest::ProductionRequest()
	{
		unitId = 0;
		objectType = TANK;
		cost = 2;
	}
	
	ProductionRequest::ProductionRequest(int unitId, ObjectType objectType, int cost)
	{
		this->unitId = unitId;
		this->objectType = objectType;
		this->cost = cost;
	}
	
	ProductionRequest::~ProductionRequest()
	{
		
	}
	
	const int ProductionRequest::getUnitId()
	{
		return unitId;
	}
	
	const ObjectType ProductionRequest::getObjectType()
	{
		return objectType;
	}
	
	const int ProductionRequest::getCost()
	{
		return cost;
	}
	
	void ProductionRequest::setUnitId(int unitId)
	{
		this->unitId = unitId;
		return;
	}
	
	void ProductionRequest::setObjectType(ObjectType objectType)
	{
		this->objectType=objectType;
		return;
	}
	
	void ProductionRequest::setCost(int cost)
	{
		this->cost = cost;
	}
	
	int ProductionRequest::updateCost(){
		--cost;
		//returning one reduces steps for startTurnUpdate
		if (cost == 0) {
			return 1;
		}else {
			return 0;
		}
	}
}