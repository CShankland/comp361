/*
 *  anotherM.cpp
 *  
 *
 *  Created by Seraphin on 10-02-26.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <deque>
#include <vector>
#include "GraphicsCommon.h"
#include "TestDriver.h"
#include "glui.h"
#include "WillsGlobals.h"
#include "Utils.h"

int mapChoice = 1;

std::string filename("");
std::string basePath("");

int playerID = 0;
bool loadMode=0;
int readyCount=0;
int playEnabled =false;

int num_of_players = 0;

bool isServer = false;

int winIdMap1;
int winIdMap2;
int winIdMap3;
int subMap1On;
int subMap2On;
int hoverMap1;
int hoverMap2;
int hoverMap3;
int subMap3On;

int chatWindow;
bool chatWindowOn=false;

Texture* maptexture1;
Texture* maptexture2;
Texture* maptexture3;

GLfloat light0_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
GLfloat light0_diffuse[] =  {.6f, .6f, 1.0f, 1.0f};
GLfloat light0_position[] = {1.0f, 0.1f, 1.0f, 0.0f};

//Very important
char playerIPInput[50];
char playerOpenMode[50];
char playerNameInput[50];
char playerChatInput[250];
char playerLimitInput[10];
int playerLimit = 4;
char c;

std::string playerName = " ";
std::string serverIP = " ";
std::string playerIP = " ";
int connected = 0;

void idle(void);

const int EXIT_MENU = 1;
const int CLEAR = -1;

const int SETUP_PLAYER = 40;
const int SETUP_NETWORK = 41;
const int JOIN_NETWORK = 42;
const int CHANGE_READY = 43;

char serverIp[250];
char serverName[250];
std::vector<std::string> netWorkInfo;  //dunno, name-ip-

//Important stuff aswell
bool deciding;
bool gettingName;
bool gettingIP;
bool gettingPlayerLimit=false;
bool gettingOpenMode = false;
bool chatting;
bool userServer;
char nameBuffer[250];

std::vector<std::string> chatHistory;
//end of really important stuff

/* Windows parameters */
#define WIDTH  1200
#define HEIGHT 600
#define TITLE  "Island Wars Main Menu"
int winIdMain;
int winIdSub;
int winIdSub11;
int winIdSub2;
int sub2on;
int winIdSub3;
int sub3on;
int winIdSub4;
int sub4on;
int winIdSubLoad;
int subLoadOn;
int winIdSubClear;
int subClearOn;
int winIdSubPlay;
int subPlayOn;
int winIdSubStatus;
int subStatusOn;

int winIdNavigator;
int navOn;
int winIdContent;
int contentOn;

int forward;

char * background;
Texture m_Texture;
Texture atexture;
clock_t current_time;
int backCount;

//Background animation variables
Texture* backTex[35];
Texture* backTex2[35];

Texture* navTga;
/* Animation State Vriables */
#define SMALL_ANGLE  5.0
#define timeB_STEP    0.1
double timeB = 0.0;
double spin = 0.0;

/* This function renders a character string
 onto the current position */
static char label[100];

//GLUUI stuff
/** These are the live variables passed into GLUI ***/
int   wireframe = 0;
int   segments = 8;
int   main_window;

int playerSetupDone = 0;
int networkSetupDone = 0;
int networkJoin = 0;
int opponentsReady = 0;

int subContent = 0;

GLUI *glui;
GLUI *glui2;
GLUI *gluiCheckList;
GLUI_EditText *tt;
GLUI_Panel obj_panel;
GLUI_Checkbox *progressCheckBox;
GLUI_Checkbox *progressNetworkSetup;
GLUI_Checkbox *progressNetworkJoin;
GLUI_Checkbox *progressOpponentsReady;

GLUI *gluiNetwork;
int gluiNetworkOn = 0;

int successfullConnect = -1;

void idle();

void actAsServer() {
	isServer = true;
	sprintf(clientHead->cname, "%s", playerNameInput);
	int preliminary = 1;	
	int turn;
	int day;
	
	int otherGuy; //the person who your directly communicating with at the moment
	std::string msg0 = "Your my bitch: We're going to Vegas!";
	std::string msg1 = "Im your bitch: (input racist remark here)";
	std::string msg2 = "Change turns";
	std::string msg3 = "End";
	
	int overallCount = 0;
	int count = 0;
	
	turn = 1;
	day = 1;
	
	int active = nextPeer();
	sendPeer(active, msg2);	
	
	while(preliminary != -1) {
		if (preliminary==1) {				
			std::string guysName = checkPeer(active);
			if (guysName != "") {
				sprintf(getCurrent()->cname, "%s", guysName.c_str());
				active = nextPeer();
				if (active == -1) {
					preliminary = 2;					
				} else {
					sendPeer(active, msg2);
				}
			}
		}
		else if (preliminary==2) {				
			sendPeerServer(getClientList());
			std::string tempList(getClientList());
			
			int pos = 0;
			while (pos >= 0) {
				if (tempList.find(playerNameInput) != std::string::npos)
					playerID++;
				pos = tempList.find(' ');
				tempList = tempList.substr(pos + 1);
				num_of_players++;
			}
			preliminary = 3;
		} else if (preliminary==3) {							
			for (int b = 0; b < 40; b++) {
				std::string exampleMap;
				for (int a = 0; a < 40; a++) {
					exampleMap += (((a*b % 3 == 0) || (a > 35))? "1" : "0");
				}
				sendPeerServer(exampleMap);
				usleep(2000);
			}
			
			std::string mapDone("Map Done");
			sendPeerServer(mapDone);
			
			preliminary = 0;
		}
		else {
			
			if (turn == 1) {
				
				if (count >= 10) {
					turn = 0;
					count = 0;
					if (++overallCount >= 4) {
						sendPeerServer(msg3);
						preliminary = -1;
					}
					
					std::string changeDaDay("Change Days");
					sendPeerServer(changeDaDay);
					
					otherGuy = nextPeer();
					sendPeer(otherGuy, msg2);
					
					day++;
				} else {
					sendPeerServer(msg0);
					count++;
				}
				
			}
			else {
				std::string listener = checkPeer(otherGuy);
				if (listener == "Change turns") {
					std::string changeDaDay("Change Days");
					
					day++;
					sendPeerServer(changeDaDay);
					
					otherGuy = nextPeer();
					if (otherGuy != -1) {
						sendPeer(otherGuy, msg2);
					} else {
						turn = 1;
					}
				} else {
					char msgToAll[50];
					sprintf(msgToAll, "%s: %s", getCurrent()->cname, listener.c_str());
					std::string outToAll(msgToAll);
					sendPeerServer(outToAll);
				}
			}
		}
	}
}

void actAsClient(std::string myClientName, int server) {
	int preliminary = 1;
	
	int otherGuy = server; //the person who your directly communicating with at the moment

	std::string msg0 = "Client: We're going to Waterloo baby!";
	std::string msg1 = "Why are you so dirty?";
	std::string msg2 = "Change turns";
	
	int count = 0;
	
	int turn = 0;
 	int day = 1;	
	
	while(preliminary != -1) {
		if (preliminary==1) {
			if (turn == 1) {
				sendPeer(otherGuy, myClientName);
				preliminary = 2;
				turn = 0;
			} else {
				std::string listener = checkPeer(otherGuy);
				if (listener == "Change turns") {
					turn = 1;
				}
			}
		}  else if (preliminary == 2) {
			std::string listener = checkPeer(otherGuy);
			std::string tempList(listener);
			int pos = 0;
			while (pos >= 0) {
				if (tempList.find(playerNameInput) != std::string::npos)
					playerID++;
				pos = tempList.find(' ');
				tempList = tempList.substr(pos + 1);
				num_of_players++;
			}
			preliminary = 3;
			
		} else if (preliminary == 3) {
			std::string listener = checkPeer(otherGuy);
			if (listener == "Map Done") {
				preliminary = 0;
			}
		} else {
			
			//testing lines of communication
			if (turn == 1) {
				if (count >= 10) {
					turn = 0;
					count = 0;
					
					day++;
					sendPeer(otherGuy, msg2);
					
				} else {
					sendPeer(otherGuy, msg0);
					count++;
				}
				
			}
			else {
				std::string listener = checkPeer(otherGuy);
				
				if (listener == "Change Days") {
					day++;
				}
				else if (listener == "Change turns") {
					turn = 1;
					
					
				} else if (listener == "End") {
					preliminary = -1;
				}
			}
		}
	}
}



void control_cb( GLUI_Control* control ){
	char buffer[250];
	switch(control->get_id()) {
		case CLEAR:
			progressCheckBox->set_int_val(0);
			progressNetworkSetup->set_int_val(0);
			progressNetworkJoin->set_int_val(0);
			progressOpponentsReady->set_int_val(0);
			break;
		case SETUP_NETWORK:
			glui->hide();
			glui->close();
			glui = NULL;
			
			
			
			progressNetworkSetup->set_int_val(1);
			myTurn = 1;
			break;
		case SETUP_PLAYER:
			glui->hide();
			
			
			progressCheckBox->set_int_val(1);
			break;
		case 1:
			if (glui != NULL) {
				glui->close();
				glui = NULL;
			}
			
			//turn off glui
			GLUI_Master.close_all();

			glutSetWindow(winIdMain);
			glutHideWindow();
			glutDestroyWindow(winIdMain);
			fMain();

			//render graphics
			break;
		case 2:
			sprintf(playerIPInput, "%s%c", playerIPInput, '2');
			break;
		case JOIN_NETWORK:
			sprintf( buffer, "%s joined from IP: %s, and is READY", playerName.c_str(), serverIP.c_str());
			netWorkInfo.push_back(std::string(buffer));
			glui->close();
			glui = NULL;
			
			progressNetworkJoin->set_int_val(1);
			connected = 1;
			myTurn = 0;
			
			break;
		default:
			break;
	}
}

void text_cb(GLUI_CB a) {
}

void 
drawString (char *s)
{
	unsigned int i;
	for (i = 0; i < strlen (s); i++)
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, s[i]);
};

void 
drawStringBig (char *s)
{
	unsigned int i;
	for (i = 0; i < strlen (s); i++)
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, s[i]);
};



/* Here is a display fucntion that updates
 the main graphic window */
void 
chatDisplay (void)
{
	if (chatWindowOn) {
		
		/* Clean drawing board */
		glutSetWindow (chatWindow);
		glClearColor (0.75, 0.0, 0.25, 0.5);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glLoadIdentity ();
		
		//put back in	
		glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
		
		
		/* Write Footnote */
		glColor3f (1.0F, 1.0F, 1.0F);
		sprintf (label, "Island Wars - Chris, Guillaume, Patrick, Simon, and Will");
		glRasterPos2f (-0.95F, -.95F);
		drawStringBig (label);

		if (backCount >= 30) backCount = 1;
		
		glPushMatrix();
		
		glTranslated(0, 0, 0);
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, backTex[backCount++]->texId);
		
		if (1)
			glColor3d(1.0, 1.0, 1.0);
		else
			glColor3d(1.0, 1.0, 1.0);
		
		glBegin(GL_QUADS);
		
		glTexCoord2d(0.0, 0.0);
		glVertex3d(-1.0, -1.0, 0.0);
		
		glTexCoord2d(1.0, 0.0);
		glVertex3d(1.0, -1.0, 0.0);
		
		glTexCoord2d(1.0, 1.0);
		glVertex3d(1.0, 1.0, 0.0);
		
		glTexCoord2d(0.0, 1.0);
		glVertex3d(-1.0, 1.0, 0.0);
		
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
		
		 if (forward) {
		 light0_ambient[0] += 0.020;
		 light0_ambient[1] += 0.020;
		 
		 light0_ambient[2] += 0.010;
		 } else {
		 light0_ambient[0] -= 0.020;
		 light0_ambient[1] -= 0.020;
		 
		 light0_ambient[2] -= 0.010;
		 }
		
		if (light0_ambient[2] > 3 || light0_ambient[2] <= 0.3)  { 
			forward = !forward;
		}
		
		glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		
		glPopMatrix();
		
		
		float yy = -1.0F;
		float xx = -0.8F;
		glColor3f (1.0F, 1.0F, 1.0F);
		
		
		if (networkSetupDone) {
			glColor3f (1.0F, 1.0F, 1.0F);
			sprintf (label, "Waiting for opponent to connect...please be Patient");
			glRasterPos2f (-0.3F, -.8F);
			drawString (label);
		}
		
		if ( successfullConnect == 1) {

			yy = -0.315F;
			xx = 0.5F;
			
			sprintf (label, "Connected To Server: %s", serverIP.c_str());
			glRasterPos2f (yy, xx);
			drawStringBig (label);	
			xx -= 0.1;
			sprintf (label, "Network Activity(Chat):");
			glRasterPos2f (yy, xx);
			drawStringBig (label);	
			xx -= 0.1;
						
			if (chatHistory.size() > 0) {
				glColor4f (1.0F, 1.0F, 1.0F,0.0f);
				int historySize = chatHistory.size();
				int spot = (( historySize>= 10)? historySize - 9 : 0);
				for (  int z = spot  ; z < historySize; z++) {
					std::string boyo = chatHistory[z];
					sprintf (label, "MadHack%s", boyo.c_str());
					glRasterPos2f (yy, xx);
					drawStringBig (label);	
					xx -= 0.1;
					
				}
			}
			
			if (chatting) {
				sprintf (label, "++\t%s_", playerChatInput);
				glRasterPos2f (yy, xx);
				drawStringBig (label);	
			} else {
				glRasterPos2f (yy, -1.3f);
				drawStringBig (playerChatInput);	
			}
		}
	}
	
	glutSwapBuffers ();
	glutPostRedisplay();
};

/* Here is a display fucntion that updates
 the main graphic window */
void mainDisplay (void)
{
	
	/* Clean drawing board */
	glutSetWindow (winIdMain);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
	
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
	
	/* Write Footnote */
	glColor3f (1.0F, 1.0F, 1.0F);
	sprintf (label, "Island Wars - Chris, Guillaume, Patrick, Simon, and Will");
	glRasterPos2f (-0.95F, -.95F);
	drawString (label);
	
	float yy = -1.0F;
	float xx = -0.8F;
	glColor3f (1.0F, 1.0F, 1.0F);
	
	if (networkSetupDone) {
		glColor3f (1.0F, 1.0F, 1.0F);
		sprintf (label, "Waiting for opponent to connect...please be Patient");
		glRasterPos2f (-0.3F, -.8F);
		drawString (label);
	}

	if ( successfullConnect == 1) {
		yy = -0.810F;
		xx = -0.4F;
		
		sprintf (label, "Connected To Server: %s", serverIP.c_str());
		glRasterPos2f (yy, xx);
		drawStringBig (label);	
		xx -= 0.1;
		sprintf (label, "Network Activity(Chat):");
		glRasterPos2f (yy, xx);
		drawStringBig (label);	
		yy = -0.315;
		
		if (chatHistory.size() > 0) {
			
			glColor4f (1.0F, 1.0F, 1.0F,0.0f);
			int historySize = chatHistory.size();
			int spot = (( historySize>= 10)? historySize - 9 : 0);
			for (  int z = spot  ; z < historySize; z++) {
				std::string boyo = chatHistory[z];
				sprintf (label, "%s", boyo.c_str());
				glRasterPos2f (yy, xx);
				drawStringBig (label);	
				xx -= 0.055;
			}
		}
		
		if (chatting) {
			sprintf (label, "++\t%s_", playerChatInput);
			glRasterPos2f (yy, xx);
			drawStringBig (label);	
		} else {
			glRasterPos2f (yy, -1.3f);
			drawStringBig (playerChatInput);	
		}
	}
	
	if (backCount >= 30) backCount = 1;

	
	glPushName(winIdMain);
	
	glPushMatrix();
	
	glTranslated(0, 0, 0);
	
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, backTex[backCount++]->texId);
	if (1)
		glColor3d(1.0, 1.0, 1.0);
	else
		glColor3d(1.0, 1.0, 1.0);
	
	glBegin(GL_QUADS);
	
	glTexCoord2d(0.0, 0.0);
	glVertex3d(-1.0, -1.0, 0.0);
	
	glTexCoord2d(1.0, 0.0);
	glVertex3d(1.0, -1.0, 0.0);
	
	glTexCoord2d(1.0, 1.0);
	glVertex3d(1.0, 1.0, 0.0);
	
	glTexCoord2d(0.0, 1.0);
	glVertex3d(-1.0, 1.0, 0.0);
	
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
	
	if (forward) {
		light0_ambient[0] += 0.020;
		light0_ambient[1] += 0.020;
		
		light0_ambient[2] += 0.010;
	} else {
		light0_ambient[0] -= 0.020;
		light0_ambient[1] -= 0.020;
		
		light0_ambient[2] -= 0.010;
	}

	if (light0_ambient[2] > 3 || light0_ambient[2] <= 0.3)  { 
		forward = !forward;
	}
	
 	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glPopMatrix();
	
	glPopName();
	glutSwapBuffers ();
	glutPostRedisplay();
};

void
navigatorDisplay ()
{
	/* Clear subwindow */
	
	glutSetWindow (winIdNavigator);
	glClearColor (0.0F, 0.1F, 0.4F, 0.3F);
	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
	
	/* Draw border */
	glColor4f (0.0F, 0.0F, 0.0F, 1.0F);
	glBegin (GL_LINE_LOOP);
	glVertex2f (0.001F, 0.001F);
	
	glVertex2f (0.001F, 0.99F);
	glColor3f (0.25F, 0.25F, 0.25F);
	
	glVertex2f (0.999F, 0.99F);
	glColor3f (0.0F, 1.0F, 0.0F);
	
	glVertex2f (0.999F, 0.001F);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	
	glEnd ();
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glColor3f (1.0F, 1.0F, 1.0F);
	sprintf (label, "Main Menu");
	glRasterPos2f (0.4F, 0.50F);
	drawStringBig (label);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	if (deciding) {
		glRasterPos2f (0.2F, 0.30F);
		drawString ("-- Enter your name and select and option below-- ");
	} else {
		if (successfullConnect == -1) {
			if (userServer) {
				glRasterPos2f (0.1F, 0.30F);
                //This fun little code snippet grabs the current comp's IP and puts it in the string strInput
                system( "ifconfig | grep broadcast > address.ffff" );
                std::ifstream inf("address.ffff");
                std::string strInput;
                inf >> strInput;
                inf >> strInput;
                inf.close();
                system( "rm address.ffff" );
                strInput.append(" is your IP This window will remain inactive until the players connect--");
				drawString (const_cast<char*> (strInput.c_str()));
			} else {
				glRasterPos2f (0.2F, 0.30F);
				drawString ("-- Enter the servers IP address and then press Enter-- ");
			}
		} 
	}
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glutSwapBuffers ();
	glutPostRedisplay();
}


void contentDisplay ()
{
	/* Clear subwindow */
	glutSetWindow (winIdContent);
	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers ();
}

void contentAnimate() {
	
	glutSetWindow(winIdContent);
	glutShowWindow();
}

void navigatorEntry(int state) {
	if (state == GLUT_ENTERED) {
		glutSetWindow (winIdNavigator);

		navOn =1;
		
		glutSwapBuffers ();
	} else if (state == GLUT_LEFT) {
		glutSetWindow (winIdNavigator);

		navOn = 0;
		
		glutSwapBuffers ();
	}
	
	glutPostRedisplay();
}

void sub2Entry(int state) {
	int currentSub = glutGetWindow();
	if (state == GLUT_ENTERED) {
		if (currentSub == winIdMap1) hoverMap1 = 1;
		else if (currentSub == winIdMap2) hoverMap2 = 1;
		else if (currentSub == winIdMap3) hoverMap3 = 1;
		else if (currentSub == winIdSub2) sub2on = 1;
		else if ( currentSub == winIdSub3 ) sub3on =1;
		else if (currentSub == winIdSub4 ) sub4on = 1;
		else if (currentSub == winIdSubLoad ) subLoadOn = 1;
		else if (currentSub == winIdSubClear ) subClearOn = 1;
		else if (currentSub == winIdSubPlay ) subPlayOn = 1;
		
		if (hoverMap1 || hoverMap2 || hoverMap3) {
			glClearColor (1.0, 1.0, 1.0, 0.3);
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
		}
		else {	
			glClearColor (0.75, 0.0, 0.25, 0.0);
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		glutSwapBuffers ();
		
	} else if (state == GLUT_LEFT) {
		if (currentSub == winIdMap1) hoverMap1 = 0;
		else if (currentSub == winIdMap2) hoverMap2 = 0;
		else if (currentSub == winIdMap3) hoverMap3 = 0;
		else if (currentSub == winIdSub2) sub2on = 0;
		else if ( currentSub == winIdSub3 ) sub3on =0;
		else if (currentSub == winIdSub4 ) sub4on = 0;
		else if (currentSub == winIdSubLoad ) subLoadOn = 0;
		else if (currentSub == winIdSubClear ) subClearOn = 0;
		else if (currentSub == winIdSubPlay ) subPlayOn = 0;
		
		if (hoverMap1 || hoverMap2 || hoverMap3) {
			glClearColor (1.0, 1.0, 1.0, 0.3);
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
		}
		else {
			glClearColor (0.25, 0.25, 0.25, 0.0);
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		glutSwapBuffers ();
	}
	glutPostRedisplay();
}

void contentExit(int state) {
	glutSwapBuffers();
}

void subDisplay2 ()
{
	/* Clear subwindow */
	glutSetWindow (winIdSub2);
	
	if (sub2on) 	glClearColor (0.75, 0.0, 0.25, 0.3);
	else glClearColor (0.25, 0.25, 0.25, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
	
	/* Draw border */
	glColor4f (0.0F, 0.0F, 0.0F, 1.0F);
	glBegin (GL_LINE_LOOP);
	glVertex2f (0.001F, 0.001F);
	
	glVertex2f (0.001F, 0.99F);
	glColor3f (0.25F, 0.25F, 0.25F);
	
	glVertex2f (0.999F, 0.99F);
	glColor3f (0.0F, 1.0F, 0.0F);
	
	glVertex2f (0.999F, 0.001F);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glEnd ();
	
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	/* Type Banner and info */
	if ( gettingName) {
		glColor3f (1.0F, 0.0F, 0.0F);
	} else {
		glColor3f (1.0F, 1.0F, 1.0F);
	}
	glRasterPos2f (0.05F, 0.30F);
	drawString ("Player Name: ");
	glRasterPos2f (0.15F, 0.30F);
	
	if (gettingName) {
		sprintf(nameBuffer, "%s_", playerNameInput);
		drawString (nameBuffer);
	}
	else {
		drawString(playerNameInput);
	}
	
	if (!deciding) {
		if (!userServer) {
			if ( gettingIP) {
				glColor3f (1.0F, 0.0F, 0.0F);
			} else {
				glColor3f (1.0F, 1.0F, 1.0F);
			}
			glRasterPos2f (0.50F, 0.30F);
			drawString ("Server IP: ");
			glRasterPos2f (0.60F, 0.30F);
			
			if (gettingIP) {
				sprintf(nameBuffer, "%s_", playerIPInput);
				drawString (nameBuffer);
			} else {
				drawString (playerIPInput);
			}
			
		} else {
			glColor3f (1.0F, 1.0F, 1.0F);
			glRasterPos2f (0.50F, 0.30F);
			drawString ("Number of Players: ");
			glRasterPos2f (0.65F, 0.30F);
			if (gettingPlayerLimit) {
				sprintf(nameBuffer, "%s_", playerLimitInput);
				drawString (nameBuffer);
			} else {
				drawString(playerLimitInput);
			}
			
			glColor3f (1.0F, 1.0F, 1.0F);
			glRasterPos2f (0.75F, 0.30F);
			drawString ("Open Mode(Y/N): ");
			glRasterPos2f (0.9F, 0.30F);
			if (gettingOpenMode) {
				sprintf(nameBuffer, "%s_", playerOpenMode);
				drawString (nameBuffer);
			} else {
				drawString(playerOpenMode);
			}
			
		}
	}
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glutSwapBuffers ();
	glutPostRedisplay();
	
}
void  subDisplay3 ()
{
	/* Clear subwindow */
	glutSetWindow (winIdSub3);
	
	if (sub3on) 	glClearColor (0.75, 0.0, 0.25, 0.3);
	else glClearColor (0.25, 0.25, 0.25, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);

	glColor4f (0.0F, 0.0F, 0.0F, 1.0F);
	glBegin (GL_LINE_LOOP);
	glVertex2f (0.001F, 0.001F);
	
	glVertex2f (0.001F, 0.99F);
	glColor3f (0.25F, 0.25F, 0.25F);
	
	glVertex2f (0.999F, 0.99F);
	glColor3f (0.0F, 1.0F, 0.0F);
	
	glVertex2f (0.999F, 0.001F);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glEnd ();
	
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	/* Type Banner and info */
	glColor3f (1.0F, 1.0F, 1.0F);
	sprintf (label, "Start Network");
	glRasterPos2f (0.422F, 0.30F);
	drawString (label);
	
	glutSwapBuffers ();
	glutPostRedisplay();
	
}


void subDisplay4 ()
{
	/* Clear subwindow */
	glutSetWindow (winIdSub4);
	
	if (sub4on) 	glClearColor (0.75, 0.0, 0.25, 0.3);
	else glClearColor (0.25, 0.25, 0.25, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
	
	/* Draw border */
	glColor4f (0.0F, 0.0F, 0.0F, 1.0F);
	glBegin (GL_LINE_LOOP);
	glVertex2f (0.001F, 0.001F);
	
	glVertex2f (0.001F, 0.99F);
	glColor3f (0.25F, 0.25F, 0.25F);
	
	glVertex2f (0.999F, 0.99F);
	glColor3f (0.0F, 1.0F, 0.0F);
	
	glVertex2f (0.999F, 0.001F);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glEnd ();
	
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	/* Type Banner and info */
	glColor3f (1.0F, 1.0F, 1.0F);
	sprintf (label, "Join Network");
	glRasterPos2f (0.424F, 0.30F);
	drawString (label);
	
	glutSwapBuffers ();
	glutPostRedisplay();
	
}


void subDisplayLoad ()
{
	/* Clear subwindow */
	glutSetWindow (winIdSubLoad);
	
	if (subLoadOn) 	glClearColor (0.75, 0.0, 0.25, 0.3);
	else glClearColor (0.25, 0.25, 0.25, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
	
	/* Draw border */
	glColor3f (0.0F, 1.0F, 0.0F);
	glBegin (GL_LINE_LOOP);
	glVertex2f (0.001F, 0.001F);
	glColor3f (0.0F, 0.5F, 0.25F);
	
	glVertex2f (0.001F, 0.99F);
	glColor3f (0.25F, 0.25F, 0.25F);
	
	glVertex2f (0.999F, 0.99F);
	glColor3f (0.0F, 1.0F, 0.0F);
	
	glVertex2f (0.999F, 0.001F);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glEnd ();
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	/* Type Banner and info */
	glColor3f (1.0F, 1.0F, 1.0F);
	sprintf (label, "Load Game: ");
	glRasterPos2f (0.422F, 0.30F);
	drawString (label);
	drawString (const_cast<char *>(filename.c_str()));
	glutSwapBuffers ();
	glutPostRedisplay();
}

void subDisplayStatus ()
{
	/* Clear subwindow */
	glutSetWindow (winIdSubStatus);
	
	if (subStatusOn) 	glClearColor (0.75, 0.0, 0.25, 0.3);
	else glClearColor (0.25, 0.25, 0.25, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);

	/* Draw border */
	glColor3f (0.0F, 1.0F, 0.0F);
	glBegin (GL_LINE_LOOP);
	glVertex2f (0.001F, 0.001F);
	glColor3f (0.0F, 0.5F, 0.25F);
	
	glVertex2f (0.001F, 0.99F);
	glColor3f (0.25F, 0.25F, 0.25F);
	
	glVertex2f (0.999F, 0.99F);
	glColor3f (0.0F, 1.0F, 0.0F);
	
	glVertex2f (0.999F, 0.001F);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glEnd ();
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	    
	/* Type Banner and info */
	if (deciding) {
		glRasterPos2f (0.3F, 0.30F);
		drawString ("-- Enter your name and select and option below-- ");
	} else {
		if (successfullConnect == -1) {
			if (userServer) {
				glRasterPos2f (0.2F, 0.30F);
				drawString ("--Press Enter to make the connection, your window will wait until finally ready-- ");
			} else {
				glRasterPos2f (0.3F, 0.30F);
				drawString ("-- Enter the servers IP address and then press Enter-- ");
			}
		} else if (successfullConnect == 1) {
			glRasterPos2f (0.3F, 0.30F);
			drawString ("-- Succesfully established connections. Press PLAY to start.-- ");
		}
	}
	
	glutSwapBuffers ();
	glutPostRedisplay();
	
}

void subDisplayClear ()
{
	/* Clear subwindow */
	glutSetWindow (winIdSubClear);
	
	if (subClearOn) 	glClearColor (0.75, 0.0, 0.25, 0.3);
	else glClearColor (0.25, 0.25, 0.25, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
	
	/* Draw border */
	glColor3f (0.0F, 1.0F, 0.0F);
	glBegin (GL_LINE_LOOP);
	glVertex2f (0.001F, 0.001F);
	glColor3f (0.0F, 0.5F, 0.25F);
	
	glVertex2f (0.001F, 0.99F);
	glColor3f (0.25F, 0.25F, 0.25F);
	
	glVertex2f (0.999F, 0.99F);
	glColor3f (0.0F, 1.0F, 0.0F);
	
	glVertex2f (0.999F, 0.001F);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glEnd ();
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	/* Type Banner and info */
	glColor3f (1.0F, 1.0F, 1.0F);
	sprintf (label, "Clear");
	glRasterPos2f (0.422F, 0.30F);
	drawString (label);
	
	glutSwapBuffers ();
	glutPostRedisplay();
}

void subDisplayPlay ()
{
	/* Clear subwindow */
	glutSetWindow (winIdSubPlay);
	
	if (subPlayOn) 	glClearColor (0.75, 0.0, 0.25, 0.3);
	else glClearColor (0.25, 0.25, 0.25, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
	
	/* Draw border */
	glColor3f (0.0F, 1.0F, 0.0F);
	glBegin (GL_LINE_LOOP);
	glVertex2f (0.001F, 0.001F);
	glColor3f (0.0F, 0.5F, 0.25F);
	
	glVertex2f (0.001F, 0.99F);
	glColor3f (0.25F, 0.25F, 0.25F);
	
	glVertex2f (0.999F, 0.99F);
	glColor3f (0.0F, 1.0F, 0.0F);
	
	glVertex2f (0.999F, 0.001F);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glEnd ();
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	/* Type Banner and info */
	glColor3f (1.0F, 1.0F, 1.0F);
	sprintf (label, "Play Game");
	glRasterPos2f (0.422F, 0.30F);
	drawString (label);
	
	glutSwapBuffers ();
	glutPostRedisplay();
}

void subMap1 ()
{
	/* Clear subwindow */
	glutSetWindow (winIdMap1);
		
	if (hoverMap1) 		glClearColor (1.0, 1.0, 1.0, 0.3);
	else if (subMap1On) glClearColor (0.75, 0.0, 0.25, 0.2);
	else glClearColor (0.0, 0.0, 0., 0.4);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
	
	/* Draw border */
	glBegin (GL_LINE_LOOP);
	glColor3f (0.0F, 1.0F, 0.25F);
	
	glVertex3d(-1.0, 1.0, 0.0);
	glColor3f (0.0F, 0.5F, 0.25F);
	
	glVertex3d(-1.0, -1.0, 0.0);
	glColor3f (0.25F, 0.25F, 0.25F);
	
	glVertex3d(1.0, -1.0, 0.0);
	glColor3f (0.0F, 1.0F, 0.0F);
	
	glVertex3d(0.99, .99, 0.0);
	glColor3f (0.0F, 1.0F, 0.25F);
	
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glEnd ();
		
	glPushName(winIdMap1);
	
	glPushMatrix();
	
	glTranslated(0, 0, 0);
	
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, maptexture1->texId );
	
	if (1)
		glColor3d(1.0, 1.0, 1.0);
	else
		glColor3d(1.0, 1.0, 1.0);
	
	glBegin(GL_QUADS);
	
	glTexCoord2d(0.0, 0.0);
	glVertex3d(-1.0, -1.0, 0.0);
	
	glTexCoord2d(1.0, 0.0);
	glVertex3d(1.0, -1.0, 0.0);
	
	glTexCoord2d(1.0, 1.0);
	glVertex3d(1.0, 1.0, 0.0);
	
	glTexCoord2d(0.0, 1.0);
	glVertex3d(-1.0, 1.0, 0.0);
	
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
 	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glPopMatrix();
	
	glPopName();
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	/* Type Banner and info */
	glColor3f (1.0F, 1.0F, 1.0F);
	sprintf (label, "Map1");
	glRasterPos2f (0.422F, 0.30F);
	drawString (label);
	
	glutSwapBuffers ();
	glutPostRedisplay();
	
	glVertex3d(1.0, -1.0, 0.0);
}

void subMap2 ()
{
	/* Clear subwindow */
	glutSetWindow (winIdMap2);
	
	glLoadIdentity();
	
	if (hoverMap2) 		glClearColor (1.0, 1.0, 1.0, 0.3);
	else if (subMap2On) glClearColor (0.75, 0.0, 0.25, 0.2);
	else glClearColor (0.0, 0.0, 0., 0.4);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
	
	/* Draw border */
	glColor3f (0.0F, 1.0F, 0.0F);
	glBegin (GL_LINE_LOOP);
	glVertex3d(-1.0, -1.0, 0.0);
	glColor3f (0.0F, 0.5F, 0.25F);
	
	glVertex3d(1.0, -1.0, 0.0);
	glColor3f (0.25F, 0.25F, 0.25F);
	
	glVertex3d(1.0, 1.0, 0.0);
	glColor3f (0.0F, 1.0F, 0.0F);
	
	glVertex3d(-1.0, 1.0, 0.0);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glEnd ();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, maptexture2->texId );
	
	if (1)
		glColor3d(1.0, 1.0, 1.0);
	else
		glColor3d(1.0, 1.0, 1.0);
	
	glBegin(GL_QUADS);
	
	glTexCoord2d(0.0, 0.0);
	glVertex3d(-1.0, -1.0, 0.0);
	
	glTexCoord2d(1.0, 0.0);
	glVertex3d(1.0, -1.0, 0.0);
	
	glTexCoord2d(1.0, 1.0);
	glVertex3d(1.0, 1.0, 0.0);
	
	glTexCoord2d(0.0, 1.0);
	glVertex3d(-1.0, 1.0, 0.0);
	
	glEnd();
	
	glDisable(GL_TEXTURE_2D);

 	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glPopMatrix();

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	/* Type Banner and info */
	glColor3f (1.0F, 1.0F, 1.0F);
	sprintf (label, "Map2");
	glRasterPos2f (0.422F, 0.30F);
	drawString (label);
	
	glutSwapBuffers ();
	glutPostRedisplay();
	
}

void subMap3 ()
{
	/* Clear subwindow */
	glutSetWindow (winIdMap3);
	
	if (hoverMap3) 		glClearColor (1.0, 1.0, 1.0, 0.3);
	else if (subMap3On) glClearColor (0.75, 0.0, 0.25, 0.2);
	else glClearColor (0.0, 0.0, 0., 0.4);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
	
	/* Draw border */
	glBegin (GL_LINE_LOOP);
	glColor3f (0.0F, 1.0F, 0.25F);
	
	glVertex3d(-1.0, 1.0, 0.0);
	glColor3f (0.0F, 0.5F, 0.25F);
	
	glVertex3d(-1.0, -1.0, 0.0);
	glColor3f (0.25F, 0.25F, 0.25F);
	
	glVertex3d(1.0, -1.0, 0.0);
	glColor3f (0.0F, 1.0F, 0.0F);
	
	glVertex3d(0.99, .99, 0.0);
	glColor3f (0.0F, 1.0F, 0.25F);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glEnd ();

	glPushName(winIdMap3);
	
	glPushMatrix();
	
	glTranslated(0, 0, 0);
	
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, maptexture3->texId );
	
	if (1)
		glColor3d(1.0, 1.0, 1.0);
	else
		glColor3d(1.0, 1.0, 1.0);
	
	glBegin(GL_QUADS);
	
	glTexCoord2d(0.0, 0.0);
	glVertex3d(-1.0, -1.0, 0.0);
	
	glTexCoord2d(1.0, 0.0);
	glVertex3d(1.0, -1.0, 0.0);
	
	glTexCoord2d(1.0, 1.0);
	glVertex3d(1.0, 1.0, 0.0);
	
	glTexCoord2d(0.0, 1.0);
	glVertex3d(-1.0, 1.0, 0.0);
	
	glEnd();
	
	glDisable(GL_TEXTURE_2D);

 	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glPopMatrix();
	
	glPopName();
		
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	/* Type Banner and info */
	glColor3f (1.0F, 1.0F, 1.0F);
	sprintf (label, "Map3");
	glRasterPos2f (0.422F, 0.30F);
	drawString (label);
	
	glutSwapBuffers ();
	glutPostRedisplay();
}


/* Another display function, this one will be 
 used to update the graphic subwindow */
void subDisplay ()
{
	/* Clear subwindow */
	glutSetWindow (winIdSub);
	
	glClearColor (0.25, 0.25, 0.25, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Draw border */
	glColor3f (0.0F, 1.0F, 0.0F);
	glBegin (GL_LINE_LOOP);
	glVertex2f (0.001F, 0.001F);
	glColor3f (0.0F, 0.5F, 0.25F);
	
	glVertex2f (0.001F, 0.99F);
	glColor3f (0.25F, 0.25F, 0.25F);
	
	glVertex2f (0.999F, 0.99F);
	glColor3f (0.0F, 1.0F, 0.0F);
	
	glVertex2f (0.999F, 0.001F);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glEnd ();
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	glColor3f (1.0F, 1.0F, 1.0F);

	/* Type Banner and info */
	glColor3f (1.0F, 1.0F, 1.0F);
	sprintf (label, " Island Wars ");
	glRasterPos2f (0.45F, 0.70F);
	drawStringBig (label);
	
	sprintf (label, " Triple Six Studios Inc.");
	glRasterPos2f (0.45F, 0.35F);
	drawString (label);
	
	if (successfullConnect == 1) {
		glRasterPos2f (-0.1F, 0.30F);
		drawString ("-- Succesfully established connections. Press PLAY to start.-- ");
		
		if (!userServer) {
			std::string reception = checkPeer(peer);
			//std::cout << reception << ", ";
			
			if (reception != "NIL") {
				chatHistory.push_back(reception);
			}		
			
			std::string dummyMessage("NIL");
			sendPeer(peer,dummyMessage);
		} else {
			sendPeerServer(std::string("NIL"));
			int currentPeer = nextPeer();
			
			while (currentPeer != -1) {
				std::string reception = checkPeer(currentPeer);
				
				if (reception != "NIL") {
					chatHistory.push_back(reception);

					usleep(50);
					
					sendPeer(currentPeer, reception);
					sendPeerServer(reception);
				} 
				
				//change to search for : as the name is always put first
				if (reception == "Ready") {
					readyCount++;
				}
				
				currentPeer = nextPeer();
			}
			
			if (readyCount == number_of_players-1) {
				playEnabled = true;
			}
		}
	}
	glutSwapBuffers ();
	glutPostRedisplay();	
};


/* Callback function for reshaping the main window */
void mainReshape (int w, int h)
{
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (-1.0F, 1.0F, -1.0F, 1.0F);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	glutSetWindow (winIdSub);
	glutReshapeWindow (w - 10, h / 10);
	glutSetWindow (winIdNavigator);
	glutReshapeWindow (w*.5,h / 8);
	glutSetWindow (winIdContent);
	glutReshapeWindow (w*.5, h / 3);
	glutPositionWindow ( (w-w*.5)/2, (h-(h/8+h/3))/2 + h / 8);
	glutSetWindow (winIdMain);
	
	if (chatWindowOn) {
		glutSetWindow (chatWindow);
		glutReshapeWindow (w - 10, h*.7);
		glutPositionWindow ((w-(w-10))/2, ((h-h*.7)/2));
	}
	
	glutSetWindow (winIdMain);
};



/* Callback function for reshaping the subwindow */
void subReshape (int w, int h)
{
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0F, 1.0F, 0.0F, 1.0F);
};


void chatReshape(int w, int h)
{
	glutSetWindow(chatWindow);
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (1.0F, -1.0F, -1.0F, 1.0F);
	
	glutShowWindow();
};


void subReshapeNavigator (int w, int h) {
	glutSetWindow(winIdNavigator);
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0F, 1.0F, 0.0F, 1.0F);
	
	glutShowWindow();
};

void
subReshapeContent (int w, int h) {
	glutSetWindow(winIdContent);
	
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0F, 1.0F, 0.0F, 1.0F);
	
	
	glutSetWindow(winIdSub2);
	glutReshapeWindow(w,h*.1);
	glutPositionWindow(0,0);
	glutSetWindow(winIdSub3);
	glutReshapeWindow(w,h*.1);
	glutPositionWindow(0,h*.1);
	glutSetWindow(winIdSub4);
	glutReshapeWindow(w,h*.1);
	glutPositionWindow(0,((h *.1)*2));
	glutSetWindow(winIdSubLoad);
	glutReshapeWindow(w,h*.1);
	glutPositionWindow(0,((h *.1)*3));
	glutSetWindow(winIdSubClear);
	glutReshapeWindow(w/2,h*.1);
	glutPositionWindow(0,((h *.1)*4));
	glutSetWindow(winIdSubPlay);
	glutReshapeWindow(w/2,h*.1);
	glutPositionWindow(w/2,((h *.1)*4));
	glutSetWindow(winIdSubStatus);
	glutReshapeWindow(w,h*.15);
	glutPositionWindow(0,((h *.1)*5));
	glutSetWindow(winIdMap1);
	glutReshapeWindow(w/3,h*.51);
	glutPositionWindow(0,((h *.1)*5));
	glutSetWindow(winIdMap2);
	glutReshapeWindow(w/3,h*.51);
	glutPositionWindow(w/3,((h *.1)*5));
	glutSetWindow(winIdMap3);
	glutReshapeWindow(w/3,h*.51);
	glutPositionWindow((w/3)*2,((h *.1)*5));
	glutSetWindow(winIdContent);
	
};

void loadKeyboard ( unsigned char key, int x, int y ) {
	if (key == 13) {
		loadMode = true;
		if (!userServer) {
			//sendPeer(peer, std::string("Ready"));
			//while (checkPeer(peer) != "Go") {
			//	sendPeer(peer, std::string("NIL"));
			//}
			
			glutSetWindow(winIdMain);
			glutHideWindow();
			glutDestroyWindow(winIdMain);
			fMain();
		}  
		else {
			if ((userServer && playEnabled)) {
				//if (userServer) {
				//	sendPeerServer(std::string("Go"));
				//}
				glutSetWindow(winIdMain);
				glutHideWindow();
				glutDestroyWindow(winIdMain);
				fMain();
			}
		}
	} else if (key == 127) {
		if (filename.length() > 0)
			filename.erase(filename.length() - 1);
	} else {
		filename += key;
	}
}

/* Now comes a function that processes keyboard events */
void 
keyboard (unsigned char key, int x, int y)
{
	static int info_banner = 1;
	if ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z')) {
		if (gettingName) {
			sprintf(playerNameInput, "%s%c", playerNameInput, key);
			
		} else if (chatting) {
			sprintf(playerChatInput, "%s%c", playerChatInput, key);
			
		} else if (gettingOpenMode) {
			if (key == 'Y' || key == 'y') {
				strcpy(playerOpenMode, "Yes");
			} else if (key == 'N' || key == 'n') {
				strcpy(playerOpenMode, "No ");
			}
		}
		
	}
	else if (key >= '0' && key <= '9') {
		if (gettingName) {
			sprintf(playerNameInput, "%s%c", playerNameInput, key);
		} else if (gettingIP) {
			sprintf(playerIPInput, "%s%c", playerIPInput, key);
		} else if (gettingPlayerLimit) {
			sprintf(playerLimitInput, "%s%c", playerLimitInput, key);
		}else if (chatting) {
			sprintf(playerChatInput, "%s%c", playerChatInput, key);
		}
	} else if (key == 46) { //point
		if (gettingName) {
			sprintf(playerNameInput, "%s.", playerNameInput, key);
		} else if (gettingIP) {
			sprintf(playerIPInput, "%s.", playerIPInput, key);
			
		} else if (chatting) {
			sprintf(playerChatInput, "%s.", key);
			
		} 
	} else if (key == 127) { //backspace
		if (gettingName) {
			if (strlen(playerNameInput) > 0) {
				memset(nameBuffer,0,250);
				
				strncpy(nameBuffer, playerNameInput, strlen(playerNameInput)-1); 
				sprintf(playerNameInput, "%s", nameBuffer);
				memset(nameBuffer,0,250);
			}
		} else if (gettingIP){
			if (strlen(playerIPInput) > 0) {
				
				memset(nameBuffer,0,250);
				
				strncpy(nameBuffer, playerIPInput, strlen(playerIPInput)-1); 
				sprintf(playerIPInput, "%s", nameBuffer);
				memset(nameBuffer,0,250);
			}
			
		} else if (gettingPlayerLimit){
			if (strlen(playerLimitInput) > 0) {
				
				memset(nameBuffer,0,250);
				
				strncpy(nameBuffer, playerLimitInput, strlen(playerLimitInput)-1); 
				sprintf(playerLimitInput, "%s", nameBuffer);
				memset(nameBuffer,0,250);
			}
			
		}else if (chatting) {
			if (strlen(playerChatInput) > 0) {
				
				memset(nameBuffer,0,250);
				
				strncpy(nameBuffer, playerChatInput, strlen(playerChatInput)-1); 
				sprintf(playerChatInput, "%s", nameBuffer);
				memset(nameBuffer,0,250);
			}			
		} 
		
	} else if (key == 9) {	//tab
		if (gettingName) {
			
			gettingName = false;
			
			if (userServer) {
				gettingPlayerLimit = true;
			} else {
				gettingIP = true;
			}
		} else if (gettingIP) {
			gettingName = true;
			gettingIP = false;
		} else if (gettingPlayerLimit) {
			gettingOpenMode = true;
			gettingPlayerLimit = false;
		} else if (gettingOpenMode) {
			gettingName = true;
			gettingOpenMode = false;
		} else if (chatting) {
			sprintf(playerChatInput, "%s\t", playerChatInput);
		
		} 
	} else if (key == 32) {	//space
		if (gettingName) {
			sprintf(playerNameInput, "%s ", playerNameInput);
		} else if (gettingIP) {
			sprintf(playerIPInput, "%s ", playerIPInput);
			
		}else if (chatting) {
			sprintf(playerChatInput, "%s ", playerChatInput);
			
		} 
	} else if (key == 13) {
		if (!deciding && successfullConnect != 1) {
			if (strlen(playerNameInput) > 0) {
				if (userServer) {
					
					startServer(atoi(playerLimitInput));
					actAsServer();
					successfullConnect = 1;
				} else {
					if (strlen(playerIPInput) > 0) {
						std::string ipAddr(playerIPInput);
						
						peer = connectClient(ipAddr);
						actAsClient(playerNameInput, peer);
						successfullConnect = 1;
						
					}
				}
			}
		} else if (chatting) {
			if (userServer) {
				if (strlen(playerChatInput) > 0) {
					std::string addName(playerNameInput);
					addName += ": ";
					std::string chatMessage(playerChatInput);
					addName += chatMessage;
					sendPeerServer(addName);
					chatHistory.push_back(addName);
					memset(playerChatInput,0,250);
				}
			} else {
				if (strlen(playerChatInput) > 0) {
					std::string addName(playerNameInput);
					addName += ": ";
					std::string chatMessage(playerChatInput);
					addName += chatMessage;
					sendPeer(peer, addName);
					memset(playerChatInput,0,250);
					
				}
			}
		}
		
	}
};


void
changeSub1(int x, int y) {
	static int sub1 = 1;
	spin = y;
	timeB = x;
	if ( x > 0 && y > 0 ) {
		if (sub1)
		{
			glutSetWindow (winIdSub);
			glutHideWindow ();
			
			glutSetWindow (chatWindow);
			glutHideWindow ();
			
			
			chatWindowOn = 0;
			glutPostRedisplay();
		}
		else
		{
			glutSetWindow (winIdSub);
			glutShowWindow ();
			
			glutSetWindow (chatWindow);
			glutShowWindow ();
			
			chatWindowOn = 1;
			
			glutPostRedisplay();
			
		};
		sub1 = !sub1;
	}
}



void
changeSub2(int x, int y) {
	glutSetWindow(winIdSub2);
	
	
	spin = y;
	timeB = x;
	glViewport (0, 0, x, y);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0F, 1.0F, 0.0F, 1.0F);
	/* Draw border */
	
	glutShowWindow();
}

void
reshapeSub3(int x, int y) {
	glutSetWindow(winIdSub3);
	
	spin = y;
	timeB = x;
	glViewport (0, 0, x, y);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0F, 1.0F, 0.0F, 1.0F);
	/* Draw border */
	
	glutShowWindow();
}

void
reshapeMap1(int x, int y) {
	glutSetWindow(winIdMap1);
	
	spin = y;
	timeB = x;
	glViewport (0, 0, x, y);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (1.0F, -1.0F, -1.0F, 1.0F);
	/* Draw border */
	
	glutShowWindow();
}
void
reshapeMap2(int x, int y) {
	glutSetWindow(winIdMap2);
	
	spin = y;
	timeB = x;
	glViewport (0, 0, x, y);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (-1.0F, 1.0F, -1.0F, 1.0F);
	/* Draw border */
	
	glutShowWindow();
}
void
reshapeMap3(int x, int y) {
	glutSetWindow(winIdMap3);
	
	spin = y;
	timeB = x;
	glViewport (0, 0, x, y);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (-1.0F, 1.0F, -1.0F, 1.0F);
	/* Draw border */
	
	glutShowWindow();
}


void
reshapeSub4(int x, int y) {
	glutSetWindow(winIdSub4);
	
	spin = y;
	timeB = x;
	glViewport (0, 0, x, y);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0F, 1.0F, 0.0F, 1.0F);
	/* Draw border */
	
	glutShowWindow();
}

void
reshapeSubLoad(int x, int y) {
	glutSetWindow(winIdSubLoad);
	
	glViewport (0, 0, x, y);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0F, 1.0F, 0.0F, 1.0F);
	/* Draw border */
	
	glutShowWindow();
}

void
reshapeSubClear(int x, int y) {
	glutSetWindow(winIdSubClear);
	
	glViewport (0, 0, x, y);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0F, 1.0F, 0.0F, 1.0F);
	/* Draw border */
	
	glutShowWindow();
}

void
reshapeSubPlay(int x, int y) {
	glutSetWindow(winIdSubPlay);
	
	glViewport (0, 0, x, y);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0F, 1.0F, 0.0F, 1.0F);
	/* Draw border */
	
	glutShowWindow();
}

void
reshapeSubStatus(int x, int y) {
	glutSetWindow(winIdSubStatus);
	
	glViewport (0, 0, x, y);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0F, 1.0F, 0.0F, 1.0F);
	/* Draw border */
	
	glutShowWindow();
}

/* There can be only one idle() callback function. In an
 animation, this idle() function must update not only the
 main window but also all derived subwindows */
void 
idle (void)
{
	
	glutSetWindow (winIdMain);
	
	glutPostRedisplay ();
	
	glutSetWindow (winIdSub);
	glutPostRedisplay ();
	
	glutSetWindow (winIdNavigator);
	glutPostRedisplay ();
	if(contentOn) {
		glutSetWindow (winIdContent);
		glutPostRedisplay ();
		glutSetWindow (winIdSub2);
		glutPostRedisplay();
		glutSetWindow (winIdSub3);
		glutPostRedisplay();
		glutSetWindow (winIdSubLoad);
		glutPostRedisplay();
		glutSetWindow (winIdSubClear);
		glutPostRedisplay();
		glutSetWindow (winIdSubPlay);
		glutPostRedisplay();
		glutSetWindow (winIdSubStatus);
		glutPostRedisplay();
		glutSetWindow (winIdMap1);
		glutPostRedisplay();
		glutSetWindow (winIdMap2);
		glutPostRedisplay();
		glutSetWindow (winIdMap3);
		glutPostRedisplay();
		glutSetWindow(winIdMain);
	}
	
	if (chatWindowOn) {
		glutSetWindow (chatWindow);
		glutShowWindow();
		glutPostRedisplay ();
	}
	
	
	
	glutPostRedisplay();

	
	
	usleep(100000);
	
};

int sub3Content = 0;
int sub2Content = 0;
void changeContent(int toWhat) {
	switch(toWhat) {
		case 1:
			glutSetWindow(winIdContent);
			
			glui->show();
			subContent = 1;
						
			break;
		case 2:
			glutSetWindow(winIdContent);
			glui->hide();
			
			glui2->show();
			
			subContent = 1;
			sub3Content = 1;
			sub2Content = 0;
			break;
		default:
			break;
	}
}

void processMouse(int button, int state, int x, int y) {
	timeB = x;
	spin =y;
	
	if ((state == GLUT_UP))		{
		// set the color to pure red for the left button
		if (button == GLUT_LEFT_BUTTON) {
			if (glutGetWindow() == winIdMap1) {
				subMap1On = !subMap1On;
				
				if (subMap1On) {
					mapChoice = 1;
				}
			} else if (glutGetWindow() == winIdMap2) {
				subMap2On = !subMap2On;
				
				if (subMap2On) {
					mapChoice = 2;
				}
			}else  if (glutGetWindow() == winIdMap3) {
				subMap3On = !subMap3On;
				
				if (subMap3On) {
					mapChoice = 3;
				}
			}
		}
	}
	
	if (glutGetWindow() == winIdSub2) {
		if ((state == GLUT_UP))		{
			// set the color to pure red for the left button
			if (button == GLUT_LEFT_BUTTON) {
				if (chatting) {
					chatting = false;
					
					gettingName = true;
					gettingPlayerLimit = false;
					gettingIP = false;
				}
				else if (gettingName && userServer && !deciding) {
					gettingName = false;
					gettingPlayerLimit = true;
				} else if (gettingName && !userServer && !deciding) {
					gettingName = false;
					gettingIP = true;
				} else if (gettingPlayerLimit) {
					gettingOpenMode  = true;
					gettingPlayerLimit = false;
				} else if (gettingIP) {
					gettingName = true;
					gettingIP = false;
				} else if (gettingOpenMode) {
					gettingName = true;
					gettingOpenMode = false;
				}
			}
		}
	} else if (glutGetWindow() == winIdNavigator) {
		
	} else if (glutGetWindow() == winIdSub || glutGetWindow() == winIdMain) {
			
			if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
				if (!chatting && !deciding) {
					chatting = true;
					
					gettingName = false;
					gettingPlayerLimit = false;
					gettingIP = false;
					
					chatWindowOn = 1;
					glutSetWindow(chatWindow);
					glutShowWindow();
					
					glutPostRedisplay();
				}
				if (glutGetWindow() == winIdMain || glutGetWindow() == winIdSub)
				{
					chatWindowOn = !chatWindowOn;

					glutSetWindow (chatWindow);
					if (chatWindowOn){ glutShowWindow ();}
					else {glutHideWindow ();}
					
				}
				else if (glutGetWindow() == winIdMain || glutGetWindow() == winIdSub) 
				{
					chatWindowOn = !chatWindowOn;

					glutSetWindow (chatWindow);
					if (chatWindowOn) {glutShowWindow ();}
					else {glutHideWindow ();}

				};
			}
	} else if (glutGetWindow() == winIdSub3) {
		if (button == GLUT_LEFT_BUTTON) {
			deciding = false;
			gettingName = false;
			gettingPlayerLimit = true;
			userServer = true;
		}	
	} else if (  glutGetWindow() == winIdSub4 ) {
		if (button == GLUT_LEFT_BUTTON) {
			deciding = false;
			gettingName = false;
			gettingIP = true;
			userServer = false;
		}
	} else if (glutGetWindow() == winIdSubClear) {
		if (successfullConnect != 1) {
			memset(playerNameInput,0,50);
			memset(playerIPInput,0,50);
			memset(playerLimitInput,0,10);
			memset(playerOpenMode,0,10);
		}
	} else if (glutGetWindow() == winIdSubPlay) {
		
		//change this to only allow play with other people.
		//i.e.  number_of_players > ~ !userUser 
			if (!userServer) {
				//sendPeer(peer, std::string("Ready"));
				//while (checkPeer(peer) != "Go") {
				//	sendPeer(peer, std::string("NIL"));
				//}
				
					glutSetWindow(winIdMain);
					glutHideWindow();
					glutDestroyWindow(winIdMain);
					fMain();
			}  
		 else {
			if ((userServer && playEnabled)) {
				//if (userServer) {
				//	sendPeerServer(std::string("Go"));
				//}
				glutSetWindow(winIdMain);
				glutHideWindow();
				glutDestroyWindow(winIdMain);
				fMain();
			}
		 }
			 
		
	} else if (glutGetWindow() == winIdSubLoad) {
				
	}
}
int 
main2 (int argc, char **argv)
{
	chatWindowOn = 0;
	loadMode = false;
	subContent = 0;
	sub2on = 0;
	sub3on = 0;
	sub4on = 0;
	subLoadOn = 0;
	subClearOn = 0;
	subPlayOn = 0;
	subStatusOn = 0;
	forward = 1;
	navOn = 0;
	contentOn = 1;
	
	strcpy(playerOpenMode, "Yes");
	gettingOpenMode = false;
	
	background = new char[100];
	
	backCount = 1;
	/* Glut initializations */
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowPosition (5, 5);
	glutInitWindowSize (WIDTH, HEIGHT);	
	
	/* Main window creation and setup */
	winIdMain = glutCreateWindow (TITLE);
	glutDisplayFunc (mainDisplay);
	glutReshapeFunc (mainReshape);
	glutIdleFunc (idle);
	glutMouseFunc(processMouse);
	glutKeyboardFunc (keyboard);
	for (int x = 0; x < 30; x++) {
		sprintf(background, "%s/Resources/%d.tga", basePath.c_str(), x+1);
		backTex[x] = createTexture(background);
	}

	navTga = createTexture("Resources/endTurn.tga");

	/****************************************/
	/*       Set up OpenGL lights           */
	/****************************************/
	
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	
	//	glEnable(GL_DEPTH_TEST);
	
	
	/* Sub window creation and setup */
	winIdSub = glutCreateSubWindow (winIdMain, 5, 5, WIDTH - 10, HEIGHT / 10);
	glutDisplayFunc (subDisplay);
	glutReshapeFunc (subReshape);
	glutMouseFunc(processMouse);
	glutKeyboardFunc (keyboard);
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	winIdNavigator = glutCreateSubWindow (winIdMain, (WIDTH-WIDTH*.5)/2, (HEIGHT-(HEIGHT/8+HEIGHT/3))/2, WIDTH*.8, HEIGHT / 8 );
	glutDisplayFunc (navigatorDisplay);
	glutReshapeFunc (subReshapeNavigator);
	glutMotionFunc(changeSub1);
	glutEntryFunc(navigatorEntry);
	glutMouseFunc(processMouse);
	
	glutKeyboardFunc (keyboard);
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	winIdContent = glutCreateSubWindow (winIdMain, (WIDTH-WIDTH*.5)/2, (HEIGHT-(HEIGHT/8+HEIGHT/3))/2 + HEIGHT / 8, WIDTH *.8, HEIGHT / 3);
	glutDisplayFunc (contentDisplay);
	glutReshapeFunc (subReshapeContent);
	glutEntryFunc(contentExit);
	
	glutKeyboardFunc (keyboard);

	//creating submenu
	winIdSub2 = glutCreateSubWindow (winIdContent, 0, 0, WIDTH *.8, (HEIGHT / 3)*.1 );

	glutDisplayFunc (subDisplay2);
	glutReshapeFunc (changeSub2);
	
	glutEntryFunc(sub2Entry);
	glutMouseFunc(processMouse);
	
	glutKeyboardFunc (keyboard);
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	winIdSub3 = glutCreateSubWindow (winIdContent, 0,(HEIGHT / 3)*.1, WIDTH *.8, (HEIGHT / 3)*.1 );
	glutDisplayFunc (subDisplay3);
	glutReshapeFunc (reshapeSub3);
	glutEntryFunc(sub2Entry);
	glutMouseFunc(processMouse);
	glutKeyboardFunc (keyboard);
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	winIdSub4 = glutCreateSubWindow (winIdContent, 0,((HEIGHT / 3)*.1)*2, WIDTH *.8, (HEIGHT / 3)*.1 );
	glutDisplayFunc (subDisplay4);
	glutReshapeFunc (reshapeSub4);
	glutEntryFunc(sub2Entry);
	glutMouseFunc(processMouse);
	glutKeyboardFunc (keyboard);
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	winIdSubLoad = glutCreateSubWindow (winIdContent, 0,((HEIGHT / 3)*.1)*3, WIDTH *.8, (HEIGHT / 3)*.1 );
	glutDisplayFunc (subDisplayLoad);
	glutReshapeFunc (reshapeSubLoad);
	glutEntryFunc(sub2Entry);
	glutMouseFunc(processMouse);
	glutKeyboardFunc (loadKeyboard);
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	winIdSubClear = glutCreateSubWindow (winIdContent, 0,((HEIGHT / 3)*.1)*4, (WIDTH *.8)/2, (HEIGHT / 3)*.1 );
	glutDisplayFunc (subDisplayClear);
	glutReshapeFunc (reshapeSubClear);
	glutEntryFunc(sub2Entry);
	glutMouseFunc(processMouse);
	glutKeyboardFunc (keyboard);
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	winIdSubPlay = glutCreateSubWindow (winIdContent, (WIDTH *.8)/2,((HEIGHT / 3)*.1)*4, (WIDTH *.8)/2, (HEIGHT / 3)*.1 );
	glutDisplayFunc (subDisplayPlay);
	glutReshapeFunc (reshapeSubPlay);
	glutEntryFunc(sub2Entry);
	glutMouseFunc(processMouse);
	glutKeyboardFunc (keyboard);
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	//map choices
	
	winIdMap1 = glutCreateSubWindow (winIdContent, 0,((HEIGHT / 3)*.1)*5, (WIDTH *.8)/3, (HEIGHT / 3)*.10 );
	glutDisplayFunc (subMap1);
	glutReshapeFunc (reshapeMap1);
	glutEntryFunc(sub2Entry);
	glutMouseFunc(processMouse);
	glutKeyboardFunc (keyboard);
	
	std::string mapTex(basePath);
	mapTex += "/Resources/land1.tga";
	maptexture1 = createTexture(mapTex.c_str());
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	winIdMap2 = glutCreateSubWindow (winIdContent, (WIDTH *.8)/3,((HEIGHT / 3)*.1)*5,  (WIDTH *.8)/3, (HEIGHT / 3)*.10 );
	glutDisplayFunc (subMap2);
	glutReshapeFunc (reshapeMap2);
	glutEntryFunc(sub2Entry);
	glutMouseFunc(processMouse);
	glutKeyboardFunc (keyboard);
	
	mapTex = std::string(basePath);
	mapTex += "/Resources/land2.tga";
	maptexture2 = createTexture(mapTex.c_str());
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	winIdMap3 = glutCreateSubWindow (winIdContent, ((WIDTH *.8)/3)*2,((HEIGHT / 3)*.1)*5,  (WIDTH *.8)/3, (HEIGHT / 3)*.10 );
	glutDisplayFunc (subMap3);
	glutReshapeFunc (reshapeMap3);
	glutEntryFunc(sub2Entry);
	glutMouseFunc(processMouse);
	glutKeyboardFunc (keyboard);
	
	mapTex = std::string(basePath);
	mapTex += "/Resources/land3.tga";
	maptexture3 = createTexture(mapTex.c_str());

	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	
	//map choices
	
	winIdSubStatus = glutCreateSubWindow (winIdContent, 0,((HEIGHT / 3)*.1)*5, WIDTH *.8, (HEIGHT / 3)*.15 );
	glutDisplayFunc (subDisplayStatus);
	glutReshapeFunc (reshapeSubStatus);
	glutEntryFunc(sub2Entry);
	glutMouseFunc(processMouse);
	glutKeyboardFunc (keyboard);
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	chatWindow = glutCreateSubWindow (winIdMain, 5, 5, WIDTH - 10, HEIGHT*.7);
	glutDisplayFunc (chatDisplay);
	glutReshapeFunc (chatReshape);
	glutMouseFunc(processMouse);
	glutKeyboardFunc (keyboard);
	
	for (int x = 0; x < 30; x++) {
		sprintf(background, "Resources/%d.tga", x+1);
		backTex2[x] = createTexture(background);
	}
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);
	
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glutSetWindow(chatWindow);
	glutHideWindow();
	
	glutSetWindow(winIdSubStatus);
	glutHideWindow();
	
	glutSetWindow(winIdMain);
	
	glutMainLoop ();
	
	return 0;
};

int main (int argc, char ** argv) {
	
	basePath = std::string(argv[0]);
	basePath = basePath.substr(0, basePath.length() - 11);

	deciding = true;
	gettingName = true;
	gettingIP = false;
	chatting = false;
	userServer = false;

	main2(0,NULL);
	return 0;
};