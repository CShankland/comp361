/*
 *  SavingsAndLoads.cpp
 *  IslandWars
 *
 *  Created by Seraphin on 10-03-24.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */



#include <sstream>
#include <iostream>
#include "SavingsAndLoads.h"
#include "GraphicsEngine.h"

namespace IslandWars {
	bool SavingsAndLoads::HandleEvent( IEventData & event ) {
		if (event.VGetEventType() == SaveGameEvent::sk_EventType) {
			writeToFile(static_cast<SaveGameEvent &>(event).m_Filename.c_str());
		}
		else {
			if (!loadMode) {
				relevantEvents.push_back(event.VSerialize());
				
				//printf( "<<");
				//printAllEvents();
				//printf(">>");
			}
		}
		return false;
	}
	const char * SavingsAndLoads::GetName( void ) {
		return "SavingsAndLoads";
	}
	void SavingsAndLoads::writeToFile(const char *filename) {
		char buffer[100];
		//std::cout << "Trying to save file";
		FILE *file = fopen(filename, "wt");
		
		
		if (file != NULL) {
			
			std::stringstream ss (std::stringstream::in | std::stringstream::out);
			ss << m_MapVal << " " << playerID << " " << mapChoice << std::endl;
			fputs(ss.str().c_str(), file);
			
			for (std::list<std::string>::iterator it = relevantEvents.begin(); it != relevantEvents.end(); it++) {
				//printf("Goood.\n");
				fputs((*it).c_str(),file);
				fputs("\n",file);
			}
			
			std::stringstream gtSS (std::stringstream::in | std::stringstream::out);
			
			BaseGraphicsEngine::getGraphicsEngine()->serializeGameTiles( gtSS );
			fputs(gtSS.str().c_str(), file);
			
			fclose(file);
			//relevantEvents.clear();
		} else {
			printf("Cannot find file");
		}
		
	}
	void SavingsAndLoads::loadGame(const char *filename) {
		char buffer[100];
		//std::cout << "Trying to load file";
		FILE *file = fopen(filename, "rt");
		
		fgets(buffer, 100, file);
		std::string is(buffer);
		//std::cout << "Loading..." << is << std::endl;
		
		//.....holy shit if statements
		
		std::stringstream ss (is, (std::stringstream::in | std::stringstream::out));
		
		ss >> m_MapVal;
		ss >> playerID;
		ss >> mapChoice;
		
		if (file != NULL) {
			while (1) {
				//printf("Goood.\n");
				fgets(buffer, 100, file);
				std::string is(buffer);
				//std::cout << "Loading..." << is << std::endl;
				
				//.....holy shit if statements
				
				std::stringstream ss (is, (std::stringstream::in | std::stringstream::out));
				
				std::string evtType;
				ss >> evtType;
				
				EventType type( evtType );
				IEventData * evt = NULL;
				
				if (type == AttackEvent::sk_EventType) {
					evt = new AttackEvent( ss );
				} else if (type == BombEvent::sk_EventType) {
					evt = new BombEvent( ss );
				} else if (type == BuildCancelledEvent::sk_EventType) {
					evt = new BuildCancelledEvent( ss );
				} else if (type == BuildEvent::sk_EventType) {
					evt = new BuildEvent( ss );
				} else if (type == FortifyEvent::sk_EventType) {
					evt = new FortifyEvent( ss );
				} else if (type == EndTurnEvent::sk_EventType) {
					evt = new EndTurnEvent( ss );
				} else if (type == BeginTurnEvent::sk_EventType) {
					evt = new BeginTurnEvent( ss );
				} else if (type == MoveUnitEvent::sk_EventType) {
					evt = new MoveUnitEvent( ss );
				} else if (type == ConquerEvent::sk_EventType) {
					evt = new ConquerEvent( ss );
				} else if (type == UnitDestroyedEvent::sk_EventType) {
					evt = new UnitDestroyedEvent( ss );
				} else if (type == ShellEvent::sk_EventType) {
					evt = new ShellEvent( ss );
				} else if (type == RepairEvent::sk_EventType) {
					evt = new RepairEvent( ss );
				} else if (type == EndGameEvent::sk_EventType) {
					evt = new EndGameEvent( ss );
				} else if (type == GameOverEvent::sk_EventType) {
					evt = new GameOverEvent( ss );
				} else if (type == SaveGameEvent::sk_EventType) {
					evt = new SaveGameEvent( ss );
				} else {
					//std::cout << "Unrecognized event" << std::endl;
					break;
				}
				
				IEventDataPtr ptr(evt);
				events.push_back(ptr);
			}
			
			std::string s(buffer);
			//std::cout << s << std::endl;
			std::stringstream fs (std::stringstream::in | std::stringstream::out);
			fs << s.c_str();
			while (!feof(file)) {
				fgets(buffer, 100, file);
				std::string t(buffer);
				fs << t;
			}
			
			BaseGraphicsEngine::getGraphicsEngine()->deserializeGameTiles( fs );
			
			fclose(file);
		} else {
			printf("Cannot find file");
		}
	}
	
}