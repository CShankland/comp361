/*
 *  ObjectFactory.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/10/10.
 *  Copyright 2010 Comp361. All rights reserved.
 
 
 * (Revision 427) 
 *  This whole file is used by Patrick.
 *
 */

#pragma once

#ifndef __OBJECT_FACTORY_H__
#define __OBJECT_FACTORY_H__

#include "Structure.h"
#include "Unit.h"

namespace IslandWars
{

	class ObjectFactory
	{
	public:
		static UnitPtr createUnit( ObjectParams * objectParams );
		static StructurePtr createStructure( ObjectParams * objectParams );
	};

}

#endif