/*  This class defines an Object,
    the most basic business logic entity on the Map.
    Author: Guillaume
    Last Edited: 26/02/10
 
 
 * (Revision 427) 
 *  This whole file is used by Patrick.
 *
 */

#include "Object.h"

namespace IslandWars {

    //There is no int to string standard function so...
    std::string intToString ( int number )
    {
		if (number < sizeof(int)*8+1) {
			char c[20];
			sprintf(c, "%d", number);
			std::string retVal(c);
			return retVal;
		}
		
		return std::string("");
		//std::ostringstream oss;
        //oss<< number;
        //return oss.str();
    }


	Object::Object( int playerId, int objectId, int x, int y, ObjectType type )
	{
		m_ObjectParams = new ObjectParams;

		m_ObjectParams->playerId = playerId;
		m_ObjectParams->objectId = objectId;
		m_ObjectParams->x_coord = x;
		m_ObjectParams->y_coord = y;
		m_ObjectParams->type = type;
	}

	Object::Object( ObjectParams * params )
	{
		m_ObjectParams = new ObjectParams;
		
		memcpy(m_ObjectParams, params, sizeof(ObjectParams));
	}

	Object::~Object()
	{
		if (m_ObjectParams)
			delete m_ObjectParams;
	}

	int Object::getId()
	{
		return m_ObjectParams->objectId;
	}

	int Object::getPlayer()
	{
		return m_ObjectParams->playerId;
	}

	void Object::setPlayer(int playerId)
	{
		m_ObjectParams->playerId = playerId;
	}

	int Object::getX()
	{
		return m_ObjectParams->x_coord;
	}

	int Object::getY()
	{
		return m_ObjectParams->y_coord;
	}

	void Object::setLocation( int x, int y )
	{
		m_ObjectParams->x_coord = x;
		m_ObjectParams->y_coord = y;
	}

	ObjectType Object::getObjectType()
	{
		return m_ObjectParams->type;
	}

	std::string Object::objectTypeToString( ObjectType type )
	{
		switch(type)
		{
			case LAND_CITY:
				return std::string("Land City");
			case PORT_CITY:
				return std::string("Port City");
			case TOWER:
				return std::string("Tower");

			case AIRFIGHTER:
				return std::string("Air Fighter");
			case BOMBER:
				return std::string("Bomber");

			case BATTLESHIP:
				return std::string("Battleship");
			case TRANSPORTER:
				return std::string("Transporter");
			case CARRIER:
				return std::string("Carrier");
			case SUBMARINE:
				return std::string("Submarine");

			case ARTILLERY:
				return std::string("Artillery");
			case FLAG:
				return std::string("Flag");
			case TANK:
				return std::string("Tank");

			default:
				return std::string("N/A");
		}
    }
}
