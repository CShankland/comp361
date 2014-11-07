/*
 *  ActorManager.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 3/28/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "ActorManager.h"
#include "GameEngine.h"
#include "WorldManager.h"
#include "glm.h"
#include "WillsGlobals.h"

#include <cmath>

namespace IslandWars {
	
#define IW_MOVE_SPEED 0.008f
#define IW_EPSION 0.2f
	
	static ActorManager * me;
	
	ActorManager::ActorManager() {
		me = this;
		m_OpenMode = false;
	}
	
	ActorManager::~ActorManager() {
		for (ViewTable::iterator it = m_ViewTable.begin(); it != m_ViewTable.end(); it++) {
			glutSetWindow(it->first);
			for (ViewData::iterator it2 = (it->second).begin(); it2 != (it->second).end(); it2++) {
				glDeleteLists((it2->second)->dl2D, 1);
				glDeleteLists((it2->second)->dl3D, 1);
			}
		}
	}
	
	ActorManager * ActorManager::get() {
		return me;
	}
	
	// --------------------------------------------------
	// Public Methods
	// --------------------------------------------------
	
	void ActorManager::registerView( int a_ViewId ) {
		ViewData viewData;
		m_ViewTable[a_ViewId] = viewData;
	}
	
	void ActorManager::addActor( ActorPtr a_Actor ) {
		
		//printf("Adding Actor\n");
		
		// Add the actor to the list of pointer
		m_ActorTable[a_Actor->getActorId()] = a_Actor;
		
		ObjectType actorType = a_Actor->getType();
		
		// Create a visual representation of the actor in all the registered views
		for (ViewTable::iterator it = m_ViewTable.begin(); it != m_ViewTable.end(); it++) {
			const int viewId = it->first;
			ViewData _data = it->second;
			
			if (_data.find(actorType) != _data.end()) {
				// There is alreaday a DrawTable for this type of actor
				// point the actor to it, and go on to the next view
				a_Actor->attachView(viewId, _data[actorType]);
				continue;
			}
			
			// Otherwise, build a new one
			DrawData * drawTable = createDrawTable(viewId, actorType);
			
			// And register it
			a_Actor->attachView(viewId, drawTable);
			_data[actorType] = drawTable;
			m_ViewTable[viewId] = _data;
		}
	}
	
	void ActorManager::removeActor( int a_ActorId ) {
		if (m_Animations.find(a_ActorId) != m_Animations.end())
			m_Animations.erase(a_ActorId);
		m_ActorTable.erase(a_ActorId);
	}
	
	void ActorManager::removeActor( ActorPtr a_Actor ) {
		if (m_Animations.find(a_Actor->getActorId()) != m_Animations.end())
			m_Animations.erase(a_Actor->getActorId());
		m_ActorTable.erase(a_Actor->getActorId());
	}
	
	ActorPtr ActorManager::getActor( int a_ActorId ) {
		if (m_ActorTable.find(a_ActorId) == m_ActorTable.end()) {
			return NullUnitActorPtr;
		}
		return m_ActorTable[a_ActorId];
	}
	
	void ActorManager::draw2D( int a_ViewId, int a_DeltaMilliseconds ) {
		
		updatePositions(a_DeltaMilliseconds);
		updateVisibility();
		
		for (ActorTable::iterator it = m_ActorTable.begin(); it != m_ActorTable.end(); it++) {
			if ((it->second)->isVisible()) {
				(it->second)->draw2D(a_ViewId, a_DeltaMilliseconds);
			}
		}
	}
	
	void ActorManager::draw3D( int a_ViewId, int a_DeltaMilliseconds ) {
		
		updateVisibility();
		updatePositions(a_DeltaMilliseconds);
		
		for (ActorTable::iterator it = m_ActorTable.begin(); it != m_ActorTable.end(); it++) {
			if ((it->second)->isVisible()) {
				(it->second)->draw3D(a_ViewId, a_DeltaMilliseconds);
			}
		}
	}
	
	void ActorManager::move( int a_ActorId, Vector2 & a_Destination ) {
		if (m_Animations.find(a_ActorId) == m_Animations.end()) {
			Itinerary itin;
			itin.push(a_Destination);
			m_Animations[a_ActorId] = itin;
		} else {
			m_Animations[a_ActorId].push(a_Destination);
		}
	}
	
	void ActorManager::setOpenMode( bool a_OpenMode ) {
		m_OpenMode = a_OpenMode;
		if (!a_OpenMode) {
			WorldManager * world = WorldManager::get();
			TileMatrix matrix = world->getTiles();
			
			for (TileMatrix::iterator it = matrix.begin(); it != matrix.end(); it++) {
				for (TileVector::iterator it2 = it->begin(); it2 != it->end(); it2++) {
					(*it2)->setVisibility(VISIBILITY_NONE);
				}
			}
			
			for (ActorTable::iterator it = m_ActorTable.begin(); it != m_ActorTable.end(); it++) {
				if ((it->second)->getPlayerId() == playerID)
					world->updateVisibility(it->second, VISIBILITY_OPEN);
			}
		}
		updateVisibility();
	}
	
	// --------------------------------------------------
	// Private Methods
	// --------------------------------------------------
	
	void ActorManager::updatePositions( int deltaMilliseconds ) {
		WorldManager * world = WorldManager::get();
		
		float scale = IW_MOVE_SPEED * deltaMilliseconds;
		
		for (AnimationRegistry::iterator it = m_Animations.begin(); it != m_Animations.end(); it++) {
			if (!it->second.empty()) {
				
				if (m_ActorTable.find(it->first) == m_ActorTable.end()) {
					while (!it->second.empty())
						it->second.pop();
					continue;
				}
				
				ActorPtr actor = m_ActorTable[it->first];
				
				GameTilePtr tile = world->getTile(it->second.front());
				
				if (actor->getPlayerId() == playerID) {
					
					Vector3 destination = tile->getDrawPos();
					
					Vector3 cur = actor->getDrawPos();
					destination.y += 0.5f;
					
					float epsilon = IW_EPSION;
					
					switch (actor->getType()) {
						case BOMBER:
						case AIRFIGHTER:
							scale *= 2;
							epsilon *= 2;
							destination.y += 1.5f;
							break;
						case SUBMARINE:
						case BATTLESHIP:
							scale *= 1.5f;
							epsilon *= 1.5f;
						case TRANSPORTER:
							destination.y += 0.5f;
						default:
							if (tile->getType() == T_SEA || tile->getType() == T_SEA2)
								destination.y -= 2.0f;
							break;
					}
					
					//printf("Destination: %f, %f, %f  Current: %f, %f, %f\n", destination.x, destination.y, destination.z, cur.x, cur.y, cur.z);
					
					float dist = 1.0f / sqrtf((destination.x - cur.x) * (destination.x - cur.x) + (destination.y - cur.y) * (destination.y - cur.y) + (destination.z - cur.z) * (destination.z - cur.z));
					
					float delX = destination.x - cur.x;
					float delY = destination.y - cur.y;
					float delZ = destination.z - cur.z;
					delX *= dist * scale; delY *= dist * scale; delZ *= dist * scale;
					
					//printf("Move stats: %f, %f, %f Delta: %d\n", delX, delY, delZ, deltaMilliseconds);
					cur.x += delX;
					cur.y += delY;
					cur.z += delZ;
					
					dist = sqrtf((destination.x - cur.x) * (destination.x - cur.x) + (destination.y - cur.y) * (destination.y - cur.y) + (destination.z - cur.z) * (destination.z - cur.z));
					
					//printf("Dist: %f\n", dist);
					if ((dist != dist) || dist < epsilon) {
						cur.x = destination.x;
						cur.y = destination.y;
						cur.z = destination.z;
					}
					
					actor->setDrawPosition(cur);
					
					if (cur.x == destination.x && cur.y == destination.y && cur.z == destination.z)
						it->second.pop();
				} else {
					if (world->getTile(it->second.back())->getVisibility() == VISIBILITY_OPEN) {
						Vector2 mapPos = it->second.front();
						while (world->getTile(mapPos)->getVisibility() != VISIBILITY_OPEN) {
							it->second.pop();
							mapPos = it->second.front();
						}
						Vector3 cur = world->getTile(mapPos)->getDrawPos();
						cur.y += 0.5f;
												
						switch (actor->getType()) {
							case BOMBER:
							case AIRFIGHTER:
								cur.y += 1.5f;
								break;
							case TRANSPORTER:
								cur.y += 0.5f;
							default:
								if (world->getTile(mapPos)->getType() == T_SEA || world->getTile(mapPos)->getType() == T_SEA2)
									cur.y -= 2.0f;
								break;
						}
						
						Vector3 destination = tile->getDrawPos();
						destination.y += 0.5f;
						
						float epsilon = IW_EPSION;
						
						switch (actor->getType()) {
							case BOMBER:
							case AIRFIGHTER:
								scale *= 2;
								epsilon *= 2;
								destination.y += 1.5f;
								break;
							case SUBMARINE:
							case BATTLESHIP:
								scale *= 1.5f;
								epsilon *= 1.5f;
							case TRANSPORTER:
								cur.y += 0.5f;
							default:
								if (tile->getType() == T_SEA || tile->getType() == T_SEA2)
									destination.y -= 2.0f;
								break;
						}
						
						//printf("Destination: %f, %f, %f  Current: %f, %f, %f\n", destination.x, destination.y, destination.z, cur.x, cur.y, cur.z);
						
						float dist = 1.0f / sqrtf((destination.x - cur.x) * (destination.x - cur.x) + (destination.y - cur.y) * (destination.y - cur.y) + (destination.z - cur.z) * (destination.z - cur.z));
						
						float delX = destination.x - cur.x;
						float delY = destination.y - cur.y;
						float delZ = destination.z - cur.z;
						delX *= dist * scale; delY *= dist * scale; delZ *= dist * scale;
						
						//printf("Move stats: %f, %f, %f Delta: %d\n", delX, delY, delZ, deltaMilliseconds);
						cur.x += delX;
						cur.y += delY;
						cur.z += delZ;
						
						dist = sqrtf((destination.x - cur.x) * (destination.x - cur.x) + (destination.y - cur.y) * (destination.y - cur.y) + (destination.z - cur.z) * (destination.z - cur.z));
						
						//printf("Dist: %f", dist);
						if ((dist != dist) || dist < epsilon) {
							cur.x = destination.x;
							cur.y = destination.y;
							cur.z = destination.z;
						}
						
						actor->setDrawPosition(cur);
						
						if (cur.x == destination.x && cur.y == destination.y && cur.z == destination.z)
							it->second.pop();
						
					}
				}
			}
		}
	}
	
	void ActorManager::updateVisibility() {
		// if (moving || the top of a stack)
		//     setVisible(true)
		// else
		//     setVisible(false)

		WorldManager * world = WorldManager::get();
		
		TileMatrix matrix = world->getTiles();
		
		if (m_OpenMode) {
			for (TileMatrix::iterator it = matrix.begin(); it != matrix.end(); it++) {
				for (TileVector::iterator it2 = it->begin(); it2 != it->end(); it2++) {
					(*it2)->setVisibility(VISIBILITY_OPEN);
					ActorList actors = (*it2)->getActors();
					if (!actors.empty()) {
						bool selected = false;
						for (ActorList::iterator it3 = actors.begin(); it3 != actors.end(); it3++) {
							if ((*it3)->isSelected()) {
								(*it3)->setVisible(true);
								selected = true;
								continue;
							}
							(*it3)->setVisible(false);
						}
						if (!selected)
							actors.front()->setVisible(true);
					}
				}
			}
		} else {
			for (TileMatrix::iterator it = matrix.begin(); it != matrix.end(); it++) {
				for (TileVector::iterator it2 = it->begin(); it2 != it->end(); it2++) {
					if ((*it2)->getVisibility() == VISIBILITY_NONE) {
						ActorList actors = (*it2)->getActors();
						if (!actors.empty()) {
							for (ActorList::iterator it3 = actors.begin(); it3 != actors.end(); it3++) {
								(*it3)->setVisible(false);
							}
						}
					} else if ((*it2)->getVisibility() == VISIBILITY_OPEN) {
						ActorList actors = (*it2)->getActors();
						if (!actors.empty()) {
							bool selected = false;
							ActorPtr submarine = NullActorPtr;
							for (ActorList::iterator it3 = actors.begin(); it3 != actors.end(); it3++) {
								if ((*it3)->getPlayerId() != playerID && (*it3)->getType() == SUBMARINE) {
									submarine = *it3;
								}
								if ((*it3)->isSelected()) {
									(*it3)->setVisible(true);
									selected = true;
									continue;
								}
								(*it3)->setVisible(false);
							}
							if (!selected) {
								if (submarine != NullActorPtr) {
									HexagonPtr p = GameEngine::get()->getHexagon(submarine->getMapPos());
									bool vis = GameEngine::get()->massEffect(p, &GameEngine::isSonar, 3);
									if (vis) {
										submarine->setVisible(true);
										break;
									}
								} else {
									actors.front()->setVisible(true);
								}
							}
						}
					}
				}
			}
		}
	}
	
	DrawData * ActorManager::createDrawTable( int a_ViewId, ObjectType a_Type ) {
		
		// Create a new DrawTable
		DrawData * table = new DrawData;
		
		// Make sure we're in the right GL context
		glutSetWindow(a_ViewId);
		
		// Display List handles
		int dl2D = glGenLists( 1 );
		
		table->dl2D = dl2D;
		//table->dl3D = dl3D;
		
		// Build textures
		Texture * tex2D = createTexture(get2DTexturePath(a_Type).c_str());
		
		// Now execute the draw code
		
		// Draw the 2D
		glNewList(dl2D, GL_COMPILE);
		
		glBindTexture(GL_TEXTURE_2D, tex2D->texId);
		buildUnitSquare();
		
		glEndList();
		
		table->dl3D = build3DActor(a_Type);
		
		/*
		// Draw the 3D
		glNewList(dl3D, GL_COMPILE);
		
		glBindTexture(GL_TEXTURE_2D, tex3D->texId);
		
		
		glEndList();
		
		 */
		// Return our table
		return table;
	}
	
	std::string ActorManager::get2DTexturePath( ObjectType a_Type ) {
		std::string ret(basePath);
		switch (a_Type) {
			case LAND_CITY:
				ret += "/Resources/LandCity.tga";
				return ret;
			case PORT_CITY:
				ret += "/Resources/PortCity.tga";
				return ret;
			case TOWER:
				ret += "/Resources/Tower.tga";
				return ret;
			case AIRFIGHTER:
				ret += "/Resources/AirFighter.tga";
				return ret;
			case BOMBER:
				ret += "/Resources/Bomber.tga";
				return ret;
			case BATTLESHIP:
				ret += "/Resources/Battleship.tga";
				return ret;
			case TRANSPORTER:
				ret += "/Resources/Transporter.tga";
				return ret;
			case CARRIER:
				ret += "/Resources/Carrier.tga";
				return ret;
			case SUBMARINE:
				ret += "/Resources/Submarine.tga";
				return ret;
			case ARTILLERY:
				ret += "/Resources/Artillery.tga";
				return ret;
			case FLAG:
				ret += "/Resources/Flag.tga";
				return ret;
			case TANK:
				ret += "/Resources/Tank.tga";
				return ret;
			default:
				return ret;
		}
	}
	
	const char * ActorManager::get3DTexturePath( ObjectType a_Type ) {
		switch (a_Type) {
			case LAND_CITY:
				return "Resources/land_city_3d.tga";
			case PORT_CITY:
				return "Resources/port_city_placeholder.tga";
			case TOWER:
				return "Resources/tower_3d.tga";
			case AIRFIGHTER:
				return "Resources/airplane_256.tga";
			case BOMBER:
				return "Resources/bomber_placeholder.tga";
			case BATTLESHIP:
				return "Resources/Battleship.tga";
			case TRANSPORTER:
				return "Resources/Transporter.tga";
			case CARRIER:
				return "Resources/carrier_placeholder.tga";
			case SUBMARINE:
				return "Resources/Submarine.tga";
			case ARTILLERY:
				return "Resources/Artillery.tga";
			case FLAG:
				return "Resources/Flag.tga";
			case TANK:
				return "Resources/Tank.tga";
			default:
				return NULL;
		}
	}
	
	// ------------------------------------------------------------------
	// Geometry Factory Functions
	// ------------------------------------------------------------------
	
	// Generate a unit sized actor based on the type
	int ActorManager::build3DActor( ObjectType a_Type ) {
		GLMmodel * model = NULL;
		char buffer[100];
		switch (a_Type) {
			case LAND_CITY:
				sprintf(buffer, "%s/Resources/city.obj", basePath.c_str());
				break;
			case PORT_CITY:
				sprintf(buffer, "%s/Resources/PortCity.obj", basePath.c_str());
				break;
			case TOWER:
				sprintf(buffer, "%s/Resources/towerWithColor.obj", basePath.c_str());
				break;
			case AIRFIGHTER:
				sprintf(buffer, "%s/Resources/fighter.obj", basePath.c_str());
				break;
			case BOMBER:
				sprintf(buffer, "%s/Resources/bomber.obj", basePath.c_str());
				break;
			case BATTLESHIP:
				sprintf(buffer, "%s/Resources/battleship.obj", basePath.c_str());
				break;
			case TRANSPORTER:
				sprintf(buffer, "%s/Resources/transporter.obj", basePath.c_str());
				break;
			case CARRIER:
				sprintf(buffer, "%s/Resources/carrier.obj", basePath.c_str());
				break;
			case SUBMARINE:
				sprintf(buffer, "%s/Resources/U48.obj", basePath.c_str());
				break;
			case ARTILLERY:
				sprintf(buffer, "%s/Resources/artillery.obj", basePath.c_str());
				break;
			case FLAG:
				sprintf(buffer, "%s/Resources/Flag.obj", basePath.c_str());
				break;
			case TANK:
				sprintf(buffer, "%s/Resources/Tank.obj", basePath.c_str());
				break;
			default:
				sprintf(buffer, "%s/Resources/towerWithColor.obj", basePath.c_str());
				break;
		}
		int ret = -1;
		model = glmReadOBJ(buffer);
		if (model) {
			glmUnitize(model);
			glmFacetNormals(model);
			glmVertexNormals(model, 90.0f);
			ret = glmList(model, GLM_SMOOTH | GLM_MATERIAL);
			glmDelete(model);
		}
		return ret;
	}
	
	// Generates a unit square in the X-Y plane, with texture coordinates
	// Centered at 0, 0, 0
	void ActorManager::buildUnitSquare() {
		
		glBegin(GL_QUADS);
		
		// TexCoord 1.0, 1.0 is the top right
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.5f, 0.5f, 0.0f);
		
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-0.5f, 0.5f, 0.0f);
		
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-0.5f, -0.5f, 0.0f);
		
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.5f, -0.5f, 0.0f);
		
		glEnd();
	}
}