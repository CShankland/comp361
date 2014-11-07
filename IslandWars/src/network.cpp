/*
 *  testNetwork.cpp
 *  GE
 *
 *  Created by Seraphin on 10-03-04.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "WillsGlobals.h"

#include "network.h"
using namespace std;

int myTurn;
int peer;

int connections[10];
int num_of_connections = 0;
int number_of_players = 0;

string myName;

ClientNode *current, *clientHead, *clientTail;
ClientNode* getCurrent() {
	return current;
}
void network_setName( string name ) {
	myName = name;
}

std::string getClientList() {
	string output;
	
	ClientNode *aptr = clientHead;
	
	while(aptr != NULL) {
		string clientName(aptr->cname);
		output += clientName;
		if (aptr->next != NULL) {
			output += " ";
		}
		
		aptr = aptr->next;
	}
	
	return output;
	
}


//the int n, signifies the number of players that are going to conncet to the server, minus itself
int startServer( int n)
{
	number_of_players = n;
	clientHead = 0;
	clientTail = 0;
	current = 0;
	
    struct sockaddr_in serverAddr;
    int sp, bound, error_check = 0, i = 0;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(9867);
    if ((sp = socket(AF_INET, SOCK_STREAM, 0)) < 0) //Socket creation error
    {
        perror("server: socket");
        exit(1);  
    }
    if (bind (sp, (struct sockaddr *) &serverAddr, sizeof(serverAddr)))//Socket bind error
    {
        perror("server: bind");
        exit(1);
    }
    setNonblocking(sp);
    if (listen(sp, 5) < 0) //Server loops here, listening for a connection no port 9860
    {
        perror("server: listen");
        exit(1);
    }
    //    for (i = 0 ; i < 500 ; i++ )
    //    {
	int connectFd = 0;
	
	//"connect" yourself
	ClientNode *cnMe = new ClientNode;
	cnMe->cpeer = -1;	
	sprintf(cnMe->cname, "%s", myName.c_str());
	
	clientHead = cnMe;
	clientTail = clientHead;
	num_of_connections++;
	
	cout << "Started game on server and Joined it:" << endl;
	
	while (num_of_connections < number_of_players) {
		connectFd = accept (sp, (struct sockaddr *) NULL, NULL);//Accepts the connection and saves
		connections[num_of_connections++] = connectFd;
		
		ClientNode *cn = new ClientNode;
		cn->cpeer = connectFd;
		
		//first if is kindof irrelevant with cnMe above present
		if (clientHead == 0) {
			clientHead = cn;
			clientTail = clientHead;
		} else {
			ClientNode *ptr;
			
			ptr = clientTail;
			
			clientTail = cn;
			clientTail->next = 0;
			ptr->next = clientTail;
		}
		
		
		//std::cout << "Client Joined" << std::endl;
	}
	//who it connected to
    
	//read list
	ClientNode *peace = clientHead;
	
	//as soon as clients join, put them in listening state
	//then once clientlist is built, iterate through ask everyone but the server
	//what is your name, one at a time, and update the clientlist with the name upon reponse
	
	/*
	 while (peace != clientTail) {
	 cout << peace->cpeer << ", ";
	 
	 
	 peace = peace->next;
	 }
	 cout <<peace->cpeer << endl;
	 
	 */
	
	while (peace != clientTail) {
		cout << peace->cpeer << ", ";
		
		peace = peace->next;
	}
	
	cout <<peace->cpeer << endl;
	current = clientHead;
	
    return current->cpeer;
	//        handleCommunication(connectFd, 1);//This is where all communication happens.
	//It passes who it's connected to and that it's its turn
	//    shutdown(connectFd, 2);
	//    close (connectFd);
    //    }
}

///////////////////////////////////CONNECTCLIENT/////////////////////////////////////////
//
//Function: This function tries to connect to a server on port 9860, with the IP passed.
//          If it is successful is goes into the handle communication loop, knowing that it
//          is the server's turn first. Otherwise, it exit(1)'s and prints the error message
//
//Parameters: string ip will me the IP address that the client attempts to connect to
//
//Returns Value: Returns nothing in successful (infinite loop), exit(1)'s if it errs
//
//Connection Port: 9860
//
////////////////////////////////////////////////////////////////////////////////////////

int connectClient(string ip)
{
    struct sockaddr_in serverAddr;
    int sp, error_check;
    if ((sp = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("client: socket");
        exit(1);
    }
    setNonblocking(sp);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    serverAddr.sin_port = htons(9867);
    if (connect(sp, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) 
    {
        perror("client: connect");
        exit(1);
    }
	cout << "Peer (in connect): " << sp;
    //handleCommunication(sp, 0);
    return sp;
    //shutdown(sp, 2);
    //close(sp);
    //return 0;
}

void handleCommunication(int peer, int my_turn)
{
    char *temp = (char*)malloc(50);//used to receive messages
    int error_check = -1, i;
    string msg;//used to send messages
    
    if (my_turn == 1)//The initial test to notify that communication is beginning
    {
        msg = "player1";
        
        send(peer, msg.c_str(), 50, 0);
        if (!receivedCheck(peer))
            return;
        cout << "Peer received packet.\nPlayer 1's turn:\n";
    }
    while (1)
    {
        if (my_turn == 1)//STATE 1
        {
            switch (i) {//Goes through 6 constant strings at the moment
                case 0:
                    msg = "MOVE ID5483 5888933 to 51231";
                    break;
                case 1:
                    msg = "MOVE ID5483 533 to 225";
                    break;
                case 2: 
                    msg = "MOVE ID5483 53 to 25";
                    break;
                case 3:
                    msg = "MOVE ID5483 3 to 225";
                    break;
                case 4: 
                    msg = "MOVE ID5483 2424 to 25";
                    break;
                case 5:
                    msg = "end";
                    break;
                default:
                    msg = "count error";
                    break;
            }
            i++;//incrementing the count to change the string
            if (msg.find("end", 0) != -1)//Toggles states at the end of a turn
                my_turn = 0;
            cout <<" Sending: " << msg << "\n";
            send (peer, msg.c_str(), 50, 0);//Send the message
            if (!receivedCheck(peer))//Wait for receipt confirmation
                return;//Return on a failed package
            cout << "  Peer received packet.\n";//Confirm receipt
        }
        else //STATE 2
        {
            cout << "Player 2's turn:\n";//yup
            error_check = -1;
            while (error_check < 0)//While there is no data
            {
                error_check = recv (peer, temp, 50, 0);//Check for new data
                if (error_check == 0)//Also check the peer hasn't died
                {
                    cout << "Server shutdown\n";
                    return;
                }
            }
            msg = temp;//The message has now been received and is stored in temp
            if (msg.find("end", 0) != -1)//Check if it's the end of a turn
            {
                i = 0;
                my_turn = 1;
            }
            cout << "Received: "<<msg<<'\n';
            msg = "received";
            send (peer, msg.c_str(), 50, 0);//Send the confirmation of receipt
            for (long j = 0; j < 500000000; j++);//Slowing it down so we can see what's going on
        }
        
    }
}

void confirm( int apeer) {
	std::string msg = "received";
	send (apeer, msg.c_str(), 50, 0);//Send the confirmation of receipt
}

int receivedCheck(int apeer)//Loops to wait for a 'received' confirmation
{
    char *temp = (char*)malloc(50);
    int i;
    int error_check = recv (apeer, temp, 50, 0);
    for (i = 0; error_check < 0 && i<100000000; i++)//Gives the peer ~1min to respond to a sent packet
    {
        error_check = recv (apeer, temp, 50, 0);
    }
    if ( i >= 100000000)
    {
        cout << "What the flux? Peer didn't respond to a sent packet.\n";
        return -1;
    }
    return 1;
}

int sendPeer(int apeer, string msg)
{
	char buff[50];
	
	sprintf(buff, "%s", msg.c_str());
    send(apeer, buff, 50, 0);
	
	
    //return receivedCheck(apeer);
}

int sendPeerServer(string msg)
{
	char buff[50];
	
	sprintf(buff, "%s", msg.c_str());
	
	ClientNode *peace = clientHead;
	
	while (peace != clientTail) {
		if (peace->cpeer != -1 && peace != current) {
			send(peace->cpeer, buff, 50, 0);
		}
		
		peace = peace->next;
	}
	
	if (peace->cpeer != -1 && peace != current)	
		send(peace->cpeer, buff, 50, 0);
	
    //return receivedCheck(apeer);
}

std::string checkPeer(int apeer)
{
	
    char temp[50];
    int error_check = recv (apeer, temp, 50, 0);
    if (error_check <= 0) {
     	//perror("server: receive");
		
		std::string out = "";
		
		return out;
	} else {
		//std::cout << "Got shit --WOoot-\n" << std::endl;
	}
	return std::string(temp);
}

std::string checkPeerServer(int apeer)
{
	
    char temp[50];
    int error_check = recv (apeer, temp, 50, 0);
    if (error_check <= 0) {
     	//perror("server: receive");
		
		std::string out = "";
		
		return out;
	} else {
		//tell everyone about it except the one who made it(created it), and the server(who is distributing it).
		string toSend(temp);
		sendPeerServer(toSend);
	}
	return std::string(temp);
}

int nextPeer() {
	if (current == clientTail) {
		current = clientHead;
	} else {
		current = current->next;
	}
	
	return current->cpeer;
}



int setNonblocking(int fd)
{
    int flags;
    
    /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
    /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    /* Otherwise, use the old way of doing it */
    flags = 1;
    //    return ioctl(fd, FIOBIO, &flags);
#endif
}
