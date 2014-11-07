/*
 *  Object.h
 *  This class declares an Object.
 *
 
 * (Revision 427) 
 *  This whole file is used by Patrick.
 *
 *
 */

#pragma once

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <sstream>
#include <string>

namespace IslandWars {


    //For some reason there is no standard int to string function in C++ (if you have one on MAC that's great but it is not standard)
    //Mostly useful for debugging purposes all over the place.
    std::string intToString ( int number );

	//Changed to accomodate all different objects
	enum ObjectType
	{
		LAND_CITY, PORT_CITY, TOWER,                 // Structures
		AIRFIGHTER, BOMBER,                          // Air Units
		BATTLESHIP, TRANSPORTER, CARRIER, SUBMARINE, // Sea Units
		ARTILLERY, FLAG, TANK                        // Land Units
	};

	class ObjectParams
	{
	public:
		ObjectParams() {};
		int playerId;
		int objectId;
		int x_coord;
		int y_coord;
		ObjectType type;
	};

	class Object
	{
	public:

		Object( int playerId, int objectId, int x, int y, ObjectType type );
		Object( ObjectParams * params );

		virtual ~Object();

		// Attribute Accessors
		int getId();
		int getPlayer();
		ObjectType getObjectType();
		int getX();
		int getY();

		// Attribute Mutators
		void setPlayer( int playerId );
		void setLocation( int x, int y );

		// Generate a description of a given ObjectType
		static std::string objectTypeToString( ObjectType type );

		// Generate a string representation of the state of an object
		//virtual std::string toString() = 0;

	protected:
		ObjectParams * m_ObjectParams;
	};

	typedef boost::shared_ptr<Object> ObjectPtr;
}

#endif