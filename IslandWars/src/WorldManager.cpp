/*
 *  WorldManager.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/28/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "WorldManager.h"
#include "Utils.h"

namespace IslandWars {
	
	static WorldManager * me;
	
	WorldManager * WorldManager::get() {
		return me;
	}
	
	WorldManager::WorldManager() {
		me = this;
	}
	
	WorldManager::~WorldManager() {
		
	}

	void WorldManager::initialize( GameMap & a_GameMap ) {
		
		float r = a_GameMap.radius;
		int w = a_GameMap.width;
		int h = a_GameMap.height;

		m_Dimensions.y = w;
		m_Dimensions.x = h;
		
		// Create the Tiles
		// Note - They do not have geometry yet
		for (int x = 0; x < h; x++) {
			TileVector tileVector;
			for (int y = 0; y < w; y++) {
				GameTile * newTile = new GameTile( Vector2( x, y ), lookupTerrain(a_GameMap.mapData[x * h + y]) );
				GameTilePtr tilePtr = GameTilePtr(newTile);
				tileVector.push_back(tilePtr);
			}
			m_Tiles.push_back(tileVector);
		}
		
		// Dimensions of the world
		float width = r * (2.0f * w - (w / 2.0f));
		float height = (h + 0.5f) * r * M_SQRT3;
		
		// Upper left corner
		float x0 = -width / 2.0f, z0 = -height / 2.0f;
		
		// Delta values
		float dz = 0.5f * r * M_SQRT3, dx = 1.5f * r;
		
		Vector3 position( x0 + r, 0.0, z0 + 0.5f * r * M_SQRT3 );
		
		// Initialize the height map, and the position of the tiles
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				int x = i, y = j;
				float * heightMap = new float[8];
				memset(heightMap, 0, sizeof(float) * 8);
				switch (m_Tiles[x][y]->getType()) {
					case T_SEA2:
					case T_SEA:
						for ( int i = 0; i < 8; i++ ) {
							heightMap[i] = -2.0f;
						}
						break;
					case T_MOUNTAIN:
						heightMap[0] = 2.0f;
						break;
					case T_LAND2:
						if (y + 1 < w) {
							if (m_Tiles[x][y + 1]->getType() == T_SEA) {
								heightMap[2] = -2.0f;
								heightMap[3] = -2.0f;
							} else if (m_Tiles[x][y + 1]->getType() == T_LAND) {
								heightMap[2] = 0.0f;
								heightMap[3] = 0.0f;
							}
						}
						
						if (y > 0) {
							if (m_Tiles[x][y - 1]->getType() == T_SEA) {
								heightMap[5] = -2.0f;
								heightMap[6] = -2.0f;
							} else if (m_Tiles[x][y - 1]->getType() == T_LAND) {
								heightMap[5] = 0.0f;
								heightMap[6] = 0.0f;
							}
						}
						
						if (x % 2 == 1)
							y--;
						
						if (x + 1 < h && y > -1) {
							if (m_Tiles[x + 1][y]->getType() == T_SEA) {
								heightMap[6] = -2.0f;
								heightMap[7] = -2.0f;
								heightMap[1] = -2.0f;
							} else if (m_Tiles[x + 1][y]->getType() == T_LAND) {
								heightMap[6] = 0.0f;
								heightMap[7] = 0.0f;
								heightMap[1] = 0.0f;
							}
						}
						
						if (x + 1 < h && y + 1 < w) {
							if (m_Tiles[x + 1][y + 1]->getType() == T_SEA) {
								heightMap[7] = -2.0f;
								heightMap[1] = -2.0f;
								heightMap[2] = -2.0f;
							} else if (m_Tiles[x + 1][y + 1]->getType() == T_LAND) {
								heightMap[7] = 0.0f;
								heightMap[1] = 0.0f;
								heightMap[2] = 0.0f;	
							}
						}
						
						if (x > 0 && y + 1 < w) {
							if (m_Tiles[x - 1][y + 1]->getType() == T_SEA) {
								heightMap[3] = -2.0f;
								heightMap[4] = -2.0f;
							} else if (m_Tiles[x - 1][y + 1]->getType() == T_LAND) {
								heightMap[3] = 0.0f;
								heightMap[4] = 0.0f;
							}
						}
						
						if (x > 0 && y > -1) {
							if (m_Tiles[x - 1][y]->getType() == T_SEA) {
								heightMap[4] = -2.0f;
								heightMap[5] = -2.0f;
							} else if (m_Tiles[x - 1][y]->getType() == T_LAND) {
								heightMap[4] = 0.0f;
								heightMap[5] = 0.0f;
							}
						}
				}
				
				m_Tiles[i][j]->initialize(position, heightMap);
				position.z += 2.0f * dz;
			}
			position.x += dx;
			position.z -= dz * w * 2.0f;
			if (i % 2 == 1)
				position.z += dz;
			else
				position.z -= dz;
		}
		
		m_HexCoords = new float[16];
		m_HexCoords[0] = 0.0f;
		m_HexCoords[1] = 0.0f;
		for (int i = 0; i < 7; i++) {
			m_HexCoords[(i + 1) * 2] = r * cosf(M_PI / 3.0f * i);
			m_HexCoords[(i + 1) * 2 + 1] = r * sinf(M_PI / 3.0f * i);
		}
		
		m_HexTex = new float[16];
		m_HexTex[ 0] = 0.500f;
		m_HexTex[ 1] = 0.435f;
		m_HexTex[ 2] = 1.000f;
		m_HexTex[ 3] = 0.435f;
		m_HexTex[ 4] = 0.750f;
		m_HexTex[ 5] = 0.870f;
		m_HexTex[ 6] = 0.250f;
		m_HexTex[ 7] = 0.870f;
		m_HexTex[ 8] = 0.000f;
		m_HexTex[ 9] = 0.435f;
		m_HexTex[10] = 0.250f;
		m_HexTex[11] = 0.000f;
		m_HexTex[12] = 0.750f;
		m_HexTex[13] = 0.000f;
		m_HexTex[14] = 1.000f;
		m_HexTex[15] = 0.435f;

	}
	
	void WorldManager::registerView( int a_ViewId ) {
		glutSetWindow( a_ViewId );
		
		int dl2D = glGenLists( 1 );
		
		glNewList( dl2D, GL_COMPILE );
		
		glBegin(GL_TRIANGLE_FAN);
		
		glVertex3f(m_HexCoords[0], -2.0f, m_HexCoords[1]);
		
		for (int i = 7; i > 0; i--) {
			glVertex3f(m_HexCoords[i * 2], -2.0f, m_HexCoords[i * 2 + 1]);
		}
		
		glEnd();
		
		glEndList();
		
		int dl3D = glGenLists( m_Dimensions.x * m_Dimensions.y );
		
		for (int x = 0; x < m_Dimensions.x; x++) {
			for (int y = 0; y < m_Dimensions.y; y++) {
				
				Vector3 drawPos = m_Tiles[x][y]->getDrawPos();
				float * heightMap = m_Tiles[x][y]->getHeightMap();
				
				// First bind the texture
				Texture * tex = createTexture(getTileTexturePath(m_Tiles[x][y]->getType()).c_str());
				
				glNewList( dl3D, GL_COMPILE );
				
				glBindTexture(GL_TEXTURE_2D, tex->texId);
				
				glBegin(GL_TRIANGLE_FAN);
				
				glTexCoord2f(m_HexTex[0], m_HexTex[1]);
				glVertex3f(drawPos.x + m_HexCoords[0], drawPos.y + heightMap[0], drawPos.z + m_HexCoords[1]);
				
				for (int i = 7; i > 0; i--) {
					glTexCoord2f(m_HexTex[i * 2], m_HexTex[i * 2 + 1]);
					glVertex3f(drawPos.x + m_HexCoords[i * 2], drawPos.y + heightMap[i], drawPos.z + m_HexCoords[i * 2 + 1]);
				}
				
				glEnd();
				
				glEndList();
				
				DrawData * drawData = new DrawData;
				drawData->dl2D = dl2D;
				drawData->dl3D = dl3D;
				
				dl3D++;
				
				m_Tiles[x][y]->attachView( a_ViewId, drawData );
			}
		}
	}
	
	void WorldManager::updateVisibility( ActorPtr a_Actor, Visibility a_Visibility ) {
		int range = 0;
		switch (a_Actor->getType()) {
			case LAND_CITY:
				range = 1;
				break;
			case PORT_CITY:
				range = 1;
				break;
			case TOWER:
				range = 2;
				break;
			case AIRFIGHTER:
				range = 2;
				break;
			case BOMBER:
				range = 3;
				break;
			case BATTLESHIP:
				range = 3;
				break;
			case TRANSPORTER:
				range = 1;
				break;
			case CARRIER:
				range = 4;
				break;
			case SUBMARINE:
				range = 3;
				break;
			case ARTILLERY:
				range = 3;
				break;
			case FLAG:
				range = 1;
				break;
			case TANK:
				range = 2;
				break;
		}
		
        Vector2 cV = a_Actor->getMapPos();
		getTile(cV)->setVisibility(a_Visibility);
        int i, j,k;
        Vector2 spinA[6], spinB[6], * spin;
        Vector2 v0(0, -1);
		
		spinA[0] = Vector2(1, 1);
		spinA[1] = Vector2(0, 1);
		spinA[2] = Vector2(-1, 0);
		spinA[3] = Vector2(-1, 0);
		spinA[4] = Vector2(0, -1);
		spinA[5] = Vector2(1, -1);
		
		spinB[0] = Vector2(1, 0);
		spinB[1] = Vector2(0, 1);
		spinB[2] = Vector2(-1, 1);
		spinB[3] = Vector2(-1, -1);
		spinB[4] = Vector2(0, -1);
		spinB[5] = Vector2(1, 0);
		
		spin = (cV.x % 2) ? spinB : spinA;
				
        for(i = 1; i <= range; i++){
            cV = cV + v0;
            for(j = 0; j < 6; j++){
                for(k = 1; k <= i; k++){
                    
					if (i > 1) {
						if ( !(i % 2) && (j == 2 || j == 5)) {
							spin = (spin == spinA) ? spinB : spinA;
						}
					}
					
					cV = cV + spin[j];
										
					if (i > 1) {
						if ( (i % 2) && (j == 2 || j == 5)) {
							spin = (spin == spinA) ? spinB : spinA;
						}
						
						if (j == 0 || j == 3) {
							spin = (spin == spinA) ? spinB : spinA;
						}
					}
					
					if (cV.x < 0 || cV.y < 0 || cV.x >= m_Dimensions.x || cV.y >= m_Dimensions.y)
						continue;
					getTile(cV)->setVisibility(a_Visibility);
                }
				if ((j == 0 || j == 2 || j == 3 || j == 5) && i > 1 && (i % 2))
					spin = (spin == spinA) ? spinB : spinA;
            }
        }
	}
	
	void WorldManager::draw2D( int a_ViewId, int a_DeltaMilliseconds ) {
		glDisable(GL_TEXTURE_2D);
		for (int i = 0; i < m_Dimensions.x; i++) {
			for (int j = 0; j < m_Dimensions.y; j++) {
				m_Tiles[i][j]->draw2D( a_ViewId, a_DeltaMilliseconds );
			}
		}
	}
	
	void WorldManager::draw3D( int a_ViewId, int a_DeltaMilliseconds ) {
		glEnable(GL_TEXTURE_2D);
		for (int i = 0; i < m_Dimensions.x; i++) {
			for (int j = 0; j < m_Dimensions.y; j++) {
				m_Tiles[i][j]->draw3D( a_ViewId, a_DeltaMilliseconds );
			}
		}
		glDisable(GL_TEXTURE_2D);
	}
	
	void WorldManager::setActorPosition( ActorPtr a_Actor, Vector2 & a_Position ) {
		m_Tiles[a_Actor->getMapPos().x][a_Actor->getMapPos().y]->removeActor( a_Actor->getActorId() );
		m_Tiles[a_Position.x][a_Position.y]->addActor( a_Actor );
		a_Actor->setMapPos( a_Position );
	}
	
	void WorldManager::removeActor( ActorPtr a_Actor ) {
		m_Tiles[a_Actor->getMapPos().x][a_Actor->getMapPos().y]->removeActor( a_Actor->getActorId() );
	}
	
	GameTilePtr WorldManager::getTile( Vector2 & a_Position ) {
		return m_Tiles[a_Position.x][a_Position.y];
	}
	
	ActorList WorldManager::getContainedActors( Vector2 & a_Position ) {
		return m_Tiles[a_Position.x][a_Position.y]->getActors();
	}
	
	std::string WorldManager::getTileTexturePath( Terrain a_Terrain ) {
		std::string ret(basePath);
		switch (a_Terrain) {
			case T_SEA:
				ret += "/Resources/HexTex_128_shallow.tga";
				return  ret;
			case T_SEA2:
				ret += "/Resources/HexTex_128_deep.tga";
				return ret;
			case T_LAND:
				ret += "/Resources/HexTex_128_forest.tga";
				return ret;
			case T_LAND2:
				ret += "/Resources/HexTex_128_beach.tga";
				return ret;
			case T_MOUNTAIN:
				ret += "/Resources/HexTex_128_mountain.tga";
				return ret;
			default:
				return ret;
		}
	}
	
	Terrain WorldManager::lookupTerrain( int mapCode ) {
		switch (mapCode) {
			case 0:
				return T_SEA2;
			case 1:
				return T_SEA;
			case 2:
				return T_LAND2;
			case 3:
				return T_LAND;
			case 4:
				return T_MOUNTAIN;
			default:
				
				//printf("MapCode: %d\n", mapCode);
				
				// not considered, but need to replace by appropriate Terrain type
				if ( mapCode%10 == 1 ){
					
					return T_LAND2; // port city on beach
					
				}else {
					
					return T_LAND; // others on land
					
				}
		}
	}
}; // Namespace IslandWars