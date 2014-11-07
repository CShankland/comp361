/* This class defines units.
   Author: Guillaume Viger
   Last Edited: 22/02/10


 * (Revision 427)
 *  This whole file is used by Patrick.


 **/

#include "Unit.h"

namespace IslandWars {

	Unit::Unit( int playerId, int objectId, int x, int y, ObjectType objectType, int maxMoves, int radarRange, int abilities, int attack, int hitpoints )
		: Object( playerId, objectId, x, y, objectType )
	{
		m_UnitParams = new UnitParams;

		m_UnitParams->maxMoves = maxMoves;
		m_UnitParams->abilities = abilities;
		m_UnitParams->remainingMoves = maxMoves;
		m_UnitParams->radarRange = radarRange;
		m_UnitParams->attack = attack;
		m_UnitParams->hitpoints = hitpoints;
		m_UnitParams->remainingHitpoints = hitpoints;
		m_UnitParams->fortified = false;
		m_UnitParams->acted = false;
		m_UnitParams->repaired = false;
	}

	Unit::Unit( ObjectParams * objectParams, UnitParams * unitParams ) : Object( objectParams )
	{
		m_UnitParams = new UnitParams;

		memcpy(m_UnitParams, unitParams, sizeof(UnitParams));
	}

	int Unit::getAbilities()
	{
		return m_UnitParams->abilities;
	}

	int Unit::getMaxMoves()
	{
		return m_UnitParams->maxMoves;
	}

	int Unit::getRemainingMoves()
	{
		return m_UnitParams->remainingMoves;
	}

	int Unit::getRadarRange()
	{
		return m_UnitParams->radarRange;
	}

	int Unit::getAttack()
	{
		return m_UnitParams->attack;
	}

	int Unit::getHitpoints()
	{
		return m_UnitParams->hitpoints;
	}

	int Unit::getRemainingHitpoints()
	{
		return m_UnitParams->remainingHitpoints;
	}

	bool Unit::isFortified()
	{
		return m_UnitParams->fortified;
	}

	bool Unit::getActed()
	{
		return m_UnitParams->acted;
	}

	bool Unit::getRepaired()
	{
		return m_UnitParams->repaired;
	}

	//Attribute Mutators

	void Unit::setRemainingMoves( int moves )
	{
	    if( moves > getMaxMoves() )
	    {
	       moves = getMaxMoves();
	    }

	    if( moves < 0 )
	    {
	       moves = 0;
	    }

	     m_UnitParams->remainingMoves = moves;
	}

	void Unit::setAttack( int attack )
	{
		m_UnitParams->attack = attack;
	}

	void Unit::setHitpoints( int hp ){
		m_UnitParams->hitpoints = hp;
	}

    //Sets hitpoints within boundaries (MaxFuel and 0)
	void Unit::setRemainingHitpoints( int hitpoints )
	{
	    if( hitpoints > getHitpoints() )
	    {
	        hitpoints = getHitpoints();
	    }

	    if( hitpoints < 0 )
	    {
	        hitpoints = 0;
	    }

        m_UnitParams->remainingHitpoints = hitpoints;
	}

	void Unit::setFortified(bool fortify)
	{
	    if( !getActed() )
	    {
			// Trying to repeat current status
			if ( ( isFortified() && fortify) || ( !isFortified() && !fortify ) ) {
				return;
				
			}
			//Defortify
			else if ( isFortified() && !fortify ) {
				setActed( true );
				setHitpoints( getHitpoints() - 1 );
				setRemainingHitpoints( getRemainingHitpoints() - 1 );
				setAttack( getAttack() - 1);
				//setRemainingMoves( 0 );
			}
			//Fortify
			else {
				setActed( true );
				setHitpoints( getHitpoints() + 1 );
				setRemainingHitpoints( getRemainingHitpoints() + 1 );
				setAttack( getAttack() + 1);
                setRemainingMoves( 0 );
			}

			m_UnitParams->fortified = fortify;
	    }
	}

	void Unit::setActed( bool acted )
	{
		m_UnitParams->acted = acted;
	}

	void Unit::setRepaired( bool repairedStatus )
	{
		m_UnitParams->repaired = repairedStatus;
	}

	std::string Unit::toString()
	{
		std::string retVal("Unit class: " + unitClassToString(getUnitClass()) +
						   "\nUnit type: " + Object::objectTypeToString(getObjectType()) +
						   "\nPlayer : " + intToString ( getPlayer() ) +
						   "\nId: " + intToString ( getId() ) +
						   "\nLocation: " + intToString ( getX() ) + ", " + intToString ( getY() ) +
						   "\nMoves: " + intToString ( getRemainingMoves() ) + " out of " + intToString ( getMaxMoves() ) +
						   "\nHitPoints: " + intToString ( getRemainingHitpoints() ) + " out of " + intToString ( getHitpoints() ) +
						   "\nAttack: " + intToString ( getAttack() ) +
						   "\nRadar range: " + intToString ( getRadarRange() ) +
						   "\nFortified: " + intToString ( isFortified() ) +
						   "\nActed : " + intToString( getActed() ));

		return retVal;
	}

	SeaUnit::SeaUnit(  int playerId, int objectId, int x, int y, ObjectType objectType, int maxMoves, int radarRange, int abilities, int attack, int hitpoints )
            :Unit(  playerId, objectId, x, y, objectType, maxMoves, radarRange, abilities, attack, hitpoints )
    { }

    SeaUnit::SeaUnit( ObjectParams * params, UnitParams * unitParams ): Unit(params, unitParams)
    { }

    UnitClass SeaUnit::getUnitClass()
    {
        return UC_SEA;
    }

    LandUnit::LandUnit(  int playerId, int objectId, int x, int y, ObjectType objectType, int maxMoves, int radarRange, int abilities, int attack, int hitpoints )
            :Unit(  playerId, objectId, x, y, objectType, maxMoves, radarRange, abilities, attack, hitpoints )
    { }

    LandUnit::LandUnit( ObjectParams * params, UnitParams * unitParams ): Unit(params, unitParams)
    { }

    UnitClass LandUnit::getUnitClass()
    {
        return UC_LAND;
    }

    AirUnit::AirUnit(  int playerId, int objectId, int x, int y, ObjectType objectType, int maxMoves, int radarRange, int abilities, int attack, int hitpoints, int maxFuel )
            :Unit(  playerId, objectId, x, y, objectType, maxMoves, radarRange, abilities, attack, hitpoints )
    {
        m_MaxFuel = maxFuel;
        setFuel( maxFuel );
    }

    AirUnit::AirUnit( ObjectParams * params, UnitParams * unitParams, int maxFuel ): Unit(params, unitParams)
    {
        m_MaxFuel = maxFuel;
        setFuel( maxFuel );
    }

    //Sets Fuel within boundaries (MaxFuel and 0)
    void AirUnit::setFuel(int fuel)
    {
        if( fuel > getMaxFuel() )
        {
            fuel = getMaxFuel();
        }

        if( fuel < 0 )
        {
            fuel = 0;
        }

        m_Fuel = fuel;
    }

    int AirUnit::getFuel()
    {
        return m_Fuel;
    }

    int AirUnit::getMaxFuel()
    {
        return m_MaxFuel;
    }

    UnitClass AirUnit::getUnitClass()
    {
        return UC_AIR ;
    }

	std::string Unit::unitClassToString( UnitClass unitClass )
	{
		switch (unitClass)
		{
			case UC_LAND:
				return std::string("Land Unit");
			case UC_SEA:
				return std::string("Sea Unit");
			case UC_AIR:
				return std::string("Air Unit");
			default:
				return std::string("Unknown Unit Class");
		}
	}
}
