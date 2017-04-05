//////////////////////////
// Filename: Server.cpp
// Author: Martin Almaraz
// Date: Tue Apr  4 18:56:38 PDT 2017
//////////////////////////


#include "Server.h"

using namespace std;

Server::Server()
{
    this->peersPort = "2888";
    this->leaderPort = "3888";
}

void Server::setServerNumber(string serverNumber)
{
    this->serverNumber = serverNumber;
}

void Server::setServerIp(string serverIp)
{
    this->serverIp = serverIp;
}

void Server::setPeersPort(string peersPort)
{
    this->peersPort = peersPort;
}

void Server::setLeaderPort(string leaderPort)
{
    this->leaderPort = leaderPort;
}

string Server::build()
{
    return "server." + this->serverNumber + "=" + this->serverIp + ":" + this->peersPort + ":" + this->leaderPort;
}
