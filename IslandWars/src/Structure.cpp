
/* This class defines structures.
 Author: Guillaume Viger
 Last Edited: 26/02/10
 
 * (Revision 427) 
 *  This whole file is used by Patrick.
 *
 */

#include "Structure.h"

namespace IslandWars {
	
	Structure::Structure( int playerId, int objectId, int x, int y, ObjectType objectType, int radarRange, ProductionCapabilities prodCaps ) : Object( playerId, objectId, x, y, objectType ), m_RadarRange( radarRange ), m_ProdCaps( prodCaps )
	{
	    m_ProdCaps = prodCaps;
		productionIndicator = 0;
		repairedThisTurn = false;
		
		garrison = std::list<UnitPtr>();
	}
	
	Structure::Structure( ObjectParams * objectParams, int radarRange, ProductionCapabilities prodCaps ) : Object( objectParams ), m_RadarRange( radarRange ), m_ProdCaps( prodCaps )
	{
	    m_ProdCaps = prodCaps;
		productionIndicator = 0;
		repairedThisTurn = false;
		
		garrison = std::list<UnitPtr>();
	}
	
	int Structure::getRadarRange()
	{
		return m_RadarRange;
	}
	
	ProductionCapabilities Structure::getProductionCapabilities()
	{
		return m_ProdCaps;
	}
	
	//How much ?
	const int Structure::getCost(ObjectType objectType){
		
		switch (objectType) {
			case TANK:
				return 4;
			case ARTILLERY:
				return 4;
			case FLAG:
				return 1;
			case BATTLESHIP:
				return 20;
			case CARRIER:
				return 10;
			case SUBMARINE:
				return 8;
			case TRANSPORTER:
				return 8;
			case AIRFIGHTER:
				return 6;
			case BOMBER:
				return 20;
			default:
				return 4;
		}
		return 0;
	}
	
	Production Structure::getProduction(){
		return production;
	}
	
	int Structure::getProductionIndicator(){
		return productionIndicator;
	}
	
	void Structure::setProductionIndicator(int productionIndicator){
		this->productionIndicator = productionIndicator;
	}
	
	// --------------------------------------------------------
	// Verify if Structure can build an Unit ?
	//
	// Return values :
	// 0 : Successful !
	// 1 : Special Case #1 : Production is full for this turn !
	// 2 : Special Case #2 : Type not supported by this type of structure !
	// --------------------------------------------------------
	int Structure::verifyBuild(ObjectType objectType, int actualTurn){
		
		//printf("Structure: actual: %d last: %d", actualTurn, production.getLastRequestTurn());
		
		bool isValidType;
		isValidType = isProductionPossible(objectType);
		
		if (isValidType) {
			
			bool isProductionFull;
			isProductionFull = production.isProductionFull(actualTurn);
			
			if (isProductionFull) {
				//Special Case #1 : Production is full for this turn !
				return 1;
			}
			
		}else {
			//Special Case #2 : Type not supported by this type of structure !
			return 2;
		}
		
		//Successful !
		return 0;
	}
	
	// --------------------------------------------------------
	// Handle production request
	// --------------------------------------------------------
	void Structure::handleProductionRequest( int unitId, ObjectType objectType, int actualTurn ){
		
		int cost;
		cost = getCost(objectType);
		
		production.addProductionRequest(unitId, objectType, cost, actualTurn);
		
		return;
		
	}
	
	
	int Structure::cancelProductionRequest(int unitId){ //if cancel old productionRequest -> check if maximum cost
		
		ProductionRequestPtr prp = production.getProductionRequest(unitId);
		
		if (prp.get() == NULL) {
			return 0;
		}
		
		ObjectType objectType = prp -> getObjectType();
		int actualCost = prp -> getCost();
		
		//allows to cancel productionRequest just ordered, and choose another one
		int maxCost = getCost(objectType);
		
		if (maxCost == actualCost) {
			production.deleteProductionRequest(unitId, 0);
		}else {
			production.deleteProductionRequest(unitId, 1);
		}
		return 1;
	}
	
	void Structure::clearProduction(){
		production.clearProductionRequests();
		return;
	}
	
	//production update at start of turn
	std::list<ProductionRequestPtr> Structure::startTurnProductionUpdate(){ //returns list of created objects
		
		//store lists of units produced
		std::list<ProductionRequestPtr> productionRequestPtrList;
		productionRequestPtrList = production.startTurnUpdate();
		
		return productionRequestPtrList;
	}
	
	bool Structure::isFreeToBuild( int actualTurn ){
		
		if (production.getLastRequestTurn() == actualTurn) {
			return false;
		}else {
			return true;
		}

	}
	
	//update costs of production requests living in production
	void Structure::endTurnProductionUpdate(){
		this->productionIndicator = production.endTurnUpdate();
		return;
	}
	
	//**** Structure Public Member Functions ****
	
	int Structure::garrisonSize(){
		return garrison.size();
	}
	
	//push unit in Garrison
	void Structure::pushGarrison( UnitPtr &up ){
		
		up -> setFortified( true );
		
		//find its right place in garrison and adds it in the list
		if (!garrison.empty()) {
			std::list<UnitPtr>::iterator iterU;
			for (iterU=garrison.begin(); iterU!=garrison.end(); ++iterU) {
				UnitPtr up_g = (*iterU);
				if (up->getRemainingHitpoints() < up_g->getRemainingHitpoints()) {
					//insert element just before the unit with greater hp
					garrison.insert(iterU,up);
					return;
				}
			}
			//all units in garrison have less or equal hp
			garrison.push_back(up);
		}else {
			//add element in garrison
			garrison.push_back(up);
		}
		
		return;
	}
	
	//pop unit from garrison
	UnitPtr Structure::topGarrison(){
		
		UnitPtr up;
		
		//check if garrison is empty
		if (garrison.empty()) {
			up = UnitPtr();
			return up;
		}else {
			up = garrison.front();
		}
		
		return up;
	}
	
	//pop unit from garrison
	UnitPtr Structure::popGarrison(){
		
		UnitPtr up;
		
		//check if garrison is empty
		if (garrison.empty()) {
			up = UnitPtr();
			return up;
		}else {
			up = garrison.front();
			
			up -> setFortified( false );
			
			garrison.pop_front();
		}
		
		return up;
	}
	
	UnitPtr Structure::getGarrisonByPosition( int position ){
		
		int i = 0;
		std::list<UnitPtr>::iterator it;
		for( it = garrison.begin(); i != position && i < garrisonSize(); ++it, ++i );
		if ( i >= garrisonSize() ){
		    //std::cout<<"No Unit at this location";
		    UnitPtr unitPtr;
            return unitPtr;
		}
		return *it;
	}
	
	//get specific unit from garrison without changing garrison
	UnitPtr Structure::getGarrisonById( int uId ){
		
		//check if garrison is empty
		if (garrison.empty()) {
			
			//empty!
			return UnitPtr();
			
		}else {
			
			//not empty !
			//UnitPtr upG;
			
			//retrieve unit with id = uId
			std::list<UnitPtr>::iterator iter;
			for (iter = garrison.begin(); iter != garrison.end(); iter++) {
				if ((*iter)->getId() == uId) {
					//UnitPtr up = upG;
					return *iter;
				}
			}
			//could not find it !
			return UnitPtr();
		}
	}
	
	//pop specific unit from garrison
	UnitPtr Structure::popGarrison( int uId ){
		
		//check if garrison is empty
		if (garrison.empty()) {
			
			//empty!
			return UnitPtr();
			
		}else {
			
			//not empty !
			UnitPtr upG;
			
			//retrieve unit with id = uId
			std::list<UnitPtr>::iterator iter;
			for (iter = garrison.begin(); iter != garrison.end(); ++iter) {
				upG = (*iter);
				if (upG->getId() == uId) {
					garrison.erase(iter);
					
					upG -> setFortified( false );
					
					return upG;
				}
			}
			return UnitPtr();
		}
	}
	
	//clear garrison ; interesting for bomber
	
	//returns the list of unit that were cleared in garrison
	//to update the Player's list
	std::list<UnitPtr> Structure::clearGarrison(){
		
		//copy, then delete
		std::list<UnitPtr> lup = garrison;
		garrison.clear();
		
		//do we need to unfortify ?
		
		return lup;
	}
	
	bool Structure::isUnderAttack(){
		
		return underAttack;
	}
	
	void Structure::setUnderAttack( bool underAttack ){
		
		this -> underAttack = underAttack;
		return;
	}
	
	// --------------------------------------------------------
	// Is object type valid at this structure ?
	// --------------------------------------------------------
	bool Structure::isProductionPossible(ObjectType objectType){
		
		//Check for the presence of objectType in list m_ProdCaps
		std::list<ObjectType>::iterator iter;
		
		for (iter=m_ProdCaps.begin(); iter != m_ProdCaps.end(); ++iter) {
			
			ObjectType o = (*iter);
			
			if (o == objectType) {
				return true;
			}
			
		}
		return false;
	}
	
	std::string Structure::getTypeToString(ObjectType objectType){
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
	
	std::string Structure::radarRangeToString() {
		
		std::string retVal("");
		char c[5];
		
		sprintf(c, "%d", m_RadarRange);
		retVal.append(c);
		
		return retVal;
	}
	
	std::string Structure::productionCapabilitiesToString(){
		
		std::string retVal("");
		
		ProductionCapabilities::iterator iter;
		for (iter = m_ProdCaps.begin(); iter != m_ProdCaps.end(); ++iter) {
			ObjectType ot = (*iter);
			retVal.append( getTypeToString(ot) );
			retVal.append( ", " );
		}
		retVal.append("-.\n");
		
		
		return retVal;
	}
	
	std::string Structure::productionToString(){
		
		std::string retVal("");
		
		if (production.getUnitsInProduction() == 0) {
			retVal.append(" empty.\n");
		}else {
			retVal.append( production.getStats() );
			retVal.append(".\n");
		}
		
		return retVal;
		
	}
	
	std::string Structure::getStats(){
		char buffer[30];
		std::string stats ("**\n\nStructure stats :\n- Type: ");
		stats.append( getTypeToString( getObjectType() ) );
		stats.append("\n");
		stats.append("- Owned by Player #");
		sprintf(buffer, "%d", getPlayer());
		stats.append( buffer );
		stats.append("\n");
		stats.append("- Id #");
		sprintf(buffer, "%d", getId());
		stats.append( buffer );
		stats.append("\n");
		stats.append("- Coordinates ");
		sprintf(buffer, "(%d,%d)", getX(), getY());
		stats.append( buffer );
		stats.append("\n");
		stats.append("- Radar Range ");
		stats.append( radarRangeToString() );
		stats.append("\n");
		stats.append("Can Produce: ");
		stats.append(productionCapabilitiesToString());
		stats.append("\n");
		stats.append("Producing: \n");
		stats.append(productionToString());
		stats.append("\n**\n");
		return stats;
	}
	
}
