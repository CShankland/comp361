//This class reads in a IWMAP file and is used to fill in the
//Hexagon Map used to represent the World Map.



#include "MapLoader.h"
#include "Events.h"
#include "GameEngine.h"
#include "WillsGlobals.h"

#include <cstring>
#include <iostream>
#include <fstream>

#ifndef DEFAULT_MAP_SIZE
#define DEFAULT_MAP_SIZE 1600
#endif

#ifndef ROW_SIZE
#define ROW_SIZE 40
#endif

#ifndef COL_SIZE
#define COL_SIZE 40
#endif

#ifndef NUM_TERRAINS
#define NUM_TERRAINS 5
#endif

namespace IslandWars {

	// --------------------------------------------------------
	// Interprets Map values stored in file
	//
	// Generate a random number for positioning structures and
	// assign terrain types to normal tiles.
	// --------------------------------------------------------	
	bool MapLoader::loadIWMAP(std::string file, int *hexArray){
		
		std::ifstream in; //reading file
		bool good = false; //verifier on file opens
		
		int i = 0; //map_size counter
		int c; //intermediate character to digit
		
		char buffer[32]; //intermediate char* to integer
		int bufIndex = 0;
		
		int randomNumber;
		srand(time(NULL));
		
		randomNumber = rand() % 5 + 1;
		
		in.open(file.c_str(), std::ifstream::in);
		
		
		//Bounded to default map size
		while(in.good() && i < ROW_SIZE*COL_SIZE){
			
			good = true;
			bufIndex = 0;
			
			while( in.peek() < '0' || in.peek() > '9' ){
				in.ignore(1);
			}
			
			c = in.get();
			buffer[bufIndex] = c;
			bufIndex++;
			
			//get extra digit
			while ( in.peek() >= '0' && in.peek() <= '9' ){
				c = in.get();
				buffer[bufIndex] = c;
				bufIndex++;
			}
			
			buffer[bufIndex] = '\0';
			
			int key = atoi(buffer);
			
			if (key >= 10){
				if (key/10 == randomNumber){
					
					// consider it
					switch (key%10) {
						case 0:
							key = 10; // tower
							break;
						case 1:
							key = 11; // PortCity
							break;
						case 2:
							key = 12; // LandCity
							break;
						case 3:
							key = 13; // starting tower
							break;
						default:
							key = 3; // land by default if no correspondings
							break;
					}
					
				} else {
					
					// not considered, but need to replace by appropriate Terrain type
					if ( key%10 == 1 ){
						
						key = 2; // port city on beach
						
					}else {
						
						key = 3; // others on land
						
					}
					
				}
				
			}
			
			// regular Terrain type
			hexArray[i] = key;
			i++;
		}
		
		in.close();
		
		return good;
	}
	
	// --------------------------------------------------------
	// Interprets Map values from chosen file
	//
	// Generate a random number for positioning structures and
	// assign terrain types to normal tiles.
	// --------------------------------------------------------
	bool MapLoader::loadIWMAP2(int choice, int num, int *hexArray){
		
		std::ifstream in; //reading file
		bool good = false; //verifier on file opens
		
		int i = 0; //map_size counter
		int c; //intermediate character to digit
		
		char buffer[32]; //intermediate char* to integer
		int bufIndex = 0;
		
		std::string file(basePath);
		
		switch (choice) {
			case 1:
				file += "/Resources/map1.IWMAP";
				break;
			case 2:
				file += "/Resources/map2.IWMAP";
				break;
			case 3:
				file += "/Resources/map3.IWMAP";
				break;
				
			default:
				file += "/Resources/map1.IWMAP";
				break;
		}
		
		in.open(file.c_str(), std::ifstream::in);
		
		
		//Bounded to default map size
		while(in.good() && i < ROW_SIZE*COL_SIZE){
			
			good = true;
			bufIndex = 0;
			
			while( in.peek() < '0' || in.peek() > '9' ){
				in.ignore(1);
			}
			
			c = in.get();
			buffer[bufIndex] = c;
			bufIndex++;
			
			//get extra digit
			while ( in.peek() >= '0' && in.peek() <= '9' ){
				c = in.get();
				buffer[bufIndex] = c;
				bufIndex++;
			}
			
			buffer[bufIndex] = '\0';
			
			int key = atoi(buffer);
			
			if (key >= 10){
				
				if (key/10 == num){
					
					//consider it
					switch (key%10) {
						case 0:
							key = 10; //tower
							break;
						case 1:
							key = 11; //PortCity
							break;
						case 2:
							key = 12; //LandCity
							break;
						case 3:
							key = 13; // starting tower
							break;
						default:
							key = 3; //land by default if no correspondings
							break;
					}
					
				} else {
					
					//not considered, but need to replace by appropriate Terrain type
					if ( key%10 == 1 ){
						
						key = 2; //port city on beach
						
					}else {
						
						key = 3; //others on land
						
					}
					
				}
				
			}
			
			//regular Terrain type
			hexArray[i] = key;
			
			i++;
			
		}
		
		
		in.close();
				
		return good;
	}
}
