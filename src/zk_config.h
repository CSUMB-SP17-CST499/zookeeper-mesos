//////////////////////////
// Filename: zk_config.h
// Author: Martin Almaraz
// Date: Tue Apr  4 18:25:19 PDT 2017
//////////////////////////

#ifndef ZK_CONFIG_H
#define ZK_CONFIG_H

#include<fstream>
#include<vector>
#include<string>
#include"Server.h"

using namespace std;

class zk_config
{
    private:
        /*
         * DEFAULT CONFIG SETTINGS
         */

        string configName;

        string tickTime;
        string dataDir;
        string clientPort;
        string initLimit;
        string syncLimit;

        int numServers;
        vector<string> servers;

    public:
        zk_config();

        void setTickTime(string tickTime);
        void setDataDir(string dataDir);
        void setClientPort(string clientPort);
        void setInitLimit(string initLimit);
        void setSyncLimit(string syncLimit);
        void setNumServers(int numServers);

        void addNode(Server server);

        void buildConfig(string configName = "zk_config.cfg");

};

#endif
