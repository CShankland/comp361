/*
 *  ProductionRequest.h
 *  IslandWarsHome
 *
 *  Created by Patrick Desmarais on 10-03-01.
 *  Copyright 2010 McGill University. All rights reserved.
 *
 
 * (Revision 427) 
 *  This whole file is used by Patrick.
 *
 */
#pragma once

#ifndef __PRODUCTION_REQUEST_H__
#define __PRODUCTION_REQUEST_H__

#include <boost/shared_ptr.hpp>
#include "Object.h"

namespace IslandWars
{

	class ProductionRequest;

	class ProductionRequest {
	private:
		int unitId;
		ObjectType objectType;
		int cost;
		
	public:
		ProductionRequest();
		ProductionRequest(int unitId, ObjectType objectType, int cost);
		~ProductionRequest();
		
		const int getUnitId();
		const ObjectType getObjectType();
		const int getCost();
		
		void setUnitId(int unitId);
		void setObjectType(ObjectType objectType);
		void setCost(int cost);
		int updateCost();
		
	};
	typedef boost::shared_ptr<ProductionRequest> ProductionRequestPtr;
	
}
#endif