/*
 *  ObjectFactory.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/10/10.
 *  Copyright 2010 Comp361. All rights reserved.
 
 * (Revision 427) 
 *  This whole file is used by Patrick.
 *
 */

#include "ObjectFactory.h"

namespace IslandWars
{

	UnitPtr ObjectFactory::createUnit( ObjectParams *objectParams )
	{
		Unit * unit = NULL;

		switch (objectParams->type)
		{
			case AIRFIGHTER:
				unit = new AirUnit( objectParams->playerId, objectParams->objectId, objectParams->x_coord, objectParams->y_coord, objectParams->type, 20,  2, ABILITY_NONE, 2, 4, 20);
				break;
			case BOMBER:
				unit = new AirUnit( objectParams->playerId, objectParams->objectId, objectParams->x_coord, objectParams->y_coord, objectParams->type, 8, 3, ABILITY_BOMB, 3, 3, 20);
				break;

			case BATTLESHIP:
				unit = new SeaUnit( objectParams->playerId, objectParams->objectId, objectParams->x_coord, objectParams->y_coord, objectParams->type, 4, 3, ABILITY_SHELL, 4, 10);
				break;
			case TRANSPORTER:
				unit = new SeaUnit( objectParams->playerId, objectParams->objectId, objectParams->x_coord, objectParams->y_coord, objectParams->type, 3, 1, ABILITY_TRANSPORT, 0, 5);
				break;
			case CARRIER:
				unit = new SeaUnit( objectParams->playerId, objectParams->objectId, objectParams->x_coord, objectParams->y_coord, objectParams->type, 2, 4, ABILITY_REFUEL, 4, 8);
				break;
			case SUBMARINE:
				unit = new SeaUnit( objectParams->playerId, objectParams->objectId, objectParams->x_coord, objectParams->y_coord, objectParams->type, 6, 3, (ABILITY_SONAR | ABILITY_SEA_ATTACK), 2, 3);
				break;

			case ARTILLERY:
				unit = new LandUnit( objectParams->playerId, objectParams->objectId, objectParams->x_coord, objectParams->y_coord, objectParams->type, 1, 3, ABILITY_SHELL, 2, 3);
				break;
			case FLAG:
				unit = new LandUnit( objectParams->playerId, objectParams->objectId, objectParams->x_coord, objectParams->y_coord, objectParams->type, 5, 1, (ABILITY_HIDE | ABILITY_COLONIZE | ABILITY_UNIQUE), 0, 2);
				break;
			case TANK:
				unit = new LandUnit( objectParams->playerId, objectParams->objectId, objectParams->x_coord, objectParams->y_coord, objectParams->type, 2, 2, ABILITY_NONE, 2, 5);
				break;
		}

		UnitPtr unitPtr(unit);
		return unitPtr;
	}

	StructurePtr ObjectFactory::createStructure( ObjectParams * objectParams )
	{
		Structure * struc = NULL;

		ProductionCapabilities prodCaps;
		if (objectParams->type == TOWER || objectParams->type == LAND_CITY || objectParams->type == PORT_CITY)
		{
			prodCaps.push_back(AIRFIGHTER);
			prodCaps.push_back(BOMBER);
			prodCaps.push_back(ARTILLERY);
			prodCaps.push_back(FLAG);
			prodCaps.push_back(TANK);

			if (objectParams->type == PORT_CITY)
			{
				prodCaps.push_back(BATTLESHIP);
				prodCaps.push_back(TRANSPORTER);
				prodCaps.push_back(CARRIER);
				prodCaps.push_back(SUBMARINE);
			}
		}

		switch (objectParams->type)
		{
			case TOWER:
			case LAND_CITY:
			case PORT_CITY:
				struc = new Structure( objectParams, 2, prodCaps );
				break;
            default:
                break;
		}

		StructurePtr strucPtr(struc);
		return strucPtr;
	}
}
