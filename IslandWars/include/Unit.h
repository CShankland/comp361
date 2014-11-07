/* A header file containing information for all units.
 Author: Guillaume Viger
 Last Edited: 24/02/10
 
 
 * (Revision 427) 
 *  This whole file is used by Patrick.
 *
 */

#pragma once

#ifndef __UNIT_H__
#define __UNIT_H__


#include "Object.h"

namespace IslandWars
{

	// Ability bit flags
	#define ABILITY_NONE       0x000
	#define ABILITY_HIDE       0x001
	#define ABILITY_COLONIZE   0x002
	#define ABILITY_SONAR      0x004
	#define ABILITY_TRANSPORT  0x008
	#define ABILITY_SHELL      0x010
	#define ABILITY_SEA_ATTACK 0x020
	#define ABILITY_UNIQUE     0x040//Not really needed anymore I think
	#define ABILITY_REFUEL     0x080
	#define ABILITY_BOMB       0x100

	enum UnitClass { UC_LAND, UC_SEA, UC_AIR };
	/*enum Order_Type{MOVING, ATTACKING, SHELLING, IDLE, ATTACKED, CONSTRUCTING};*/

	struct Result;

	typedef struct
	{
		int maxMoves;
		int abilities;
		int remainingMoves;
		int radarRange;
		int attack;
		int hitpoints;
        int remainingHitpoints;
		bool fortified;
		bool acted;
		bool repaired;
	} UnitParams;

	class Unit : public Object{

    public:

        Unit( int playerId, int objectId, int x, int y, ObjectType objectType, int maxMoves, int radarRange, int abilities, int attack, int hitpoints );
		Unit( ObjectParams * objectParams, UnitParams * unitParams );

		// Attribute Accessors
		int getAbilities();
		int getMaxMoves();
		int getRemainingMoves();
		int getRadarRange();
		int getAttack();
		int getHitpoints();
		int getRemainingHitpoints();
		bool isFortified();
		bool getActed();
		bool getRepaired();

		// Attribute Mutators
		void setRemainingMoves( int moves );
		void setAttack( int attack );
		void setRemainingHitpoints( int hp );
		void setHitpoints( int hp );
		void setFortified( bool fortify );
		void setActed( bool acted );
		void setRepaired( bool repairedStatus );

		 //Required by Object
		virtual std::string toString();

		// Pure Virtuals
		virtual UnitClass getUnitClass() = 0;

        //Debugging Functions
		static std::string unitClassToString( UnitClass unitClass );


	protected:

		UnitParams * m_UnitParams;

	};

	typedef boost::shared_ptr<Unit> UnitPtr;

	struct Result{
		bool unit_destroyed;
		Unit *destr_Unit;
	};

	class SeaUnit : public Unit
	{
	public:

		SeaUnit(  int playerId, int objectId, int x, int y, ObjectType objectType, int maxMoves, int radarRange, int abilities, int attack, int hitpoints );
		SeaUnit( ObjectParams * params, UnitParams * unitParams );

		UnitClass getUnitClass();
	};

	class LandUnit : public Unit
	{
	public:

		LandUnit(  int playerId, int objectId, int x, int y, ObjectType objectType, int maxMoves, int radarRange, int abilities, int attack, int hitpoints );
		LandUnit( ObjectParams * params, UnitParams * unitParams );

		UnitClass getUnitClass();
	};

	class AirUnit : public Unit
	{
	public:

		AirUnit( int playerId, int objectId, int x, int y, ObjectType objectType, int maxMoves, int radarRange, int abilities, int attack, int hitpoints, int maxFuel );
		AirUnit( ObjectParams * params, UnitParams * unitParams, int maxFuel );

		UnitClass getUnitClass();
		//virtual std::string toString();

		int getMaxFuel();
		int getFuel();
		void setFuel( int fuel);

	private:

		int m_MaxFuel;
		int m_Fuel;
	};

	typedef boost::shared_ptr<SeaUnit> SeaUnitPtr;
	typedef boost::shared_ptr<LandUnit> LandUnitPtr;
	typedef boost::shared_ptr<AirUnit> AirUnitPtr;
}

#endif
