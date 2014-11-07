/*
 *  testNetwork.h
 *  GE
 *
 *  Created by Seraphin on 10-03-04.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#ifndef _NETWORK_H_
#define _NETWORK_H_
#include <iostream>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <cstdlib>     /* for atoi() and exit() */
#include <string>     /* for memset() */
#include <unistd.h>     /* for close() */

extern int myTurn;
extern int peer;

extern char test_name[50];

extern int connections[10];
extern int num_of_connections;
extern int number_of_players;

extern std::string myName;

struct ClientNode{
	char cname[50];
	int cpeer;
	struct ClientNode *next;
	bool alive;
};

extern ClientNode *current, *clientHead, *clientTail;
ClientNode* getCurrent();

std::string getClientList();

int startServer(int);
int connectClient(std::string);
void handleCommunication(int, int);
int sendPeer(int, std::string);
int sendPeerServer(std::string);

std::string checkPeer(int);

std::string checkPeerServer(int);

std::string getMove();
int receivedCheck(int);
void confirm(int apeer);
int receivedCheck(int);
int setNonblocking(int);
int nextPeer();
#endif
