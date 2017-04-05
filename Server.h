//////////////////////////
// Filename: Server.h
// Author: Martin Almaraz
// Date: Tue Apr  4 18:56:38 PDT 2017
//////////////////////////

#ifndef SERVER_H
#define SERVER_H

#include<string>
using namespace std;

class Server
{
    private:
        string serverNumber;
        string serverIp;
        string peersPort;
        string leaderPort;

    public:
        Server();
        string build();

        void setServerNumber(string serverNumber);
        void setServerIp(string serverIp);
        void setPeersPort(string peersPort);
        void setLeaderPort(string leaderPort);
};

#endif
