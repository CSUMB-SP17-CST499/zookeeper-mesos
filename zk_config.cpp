//////////////////////////
// Filename: zk_config.cpp
// Author: Martin Almaraz
// Date: Tue Apr  4 18:25:19 PDT 2017
//////////////////////////


#include "zk_config.h"
#include "Server.h"

using namespace std;

zk_config::zk_config()
{
    this->tickTime = "2000";
    this->dataDir = "/var/lib/zookeeper/";
    this->clientPort = "2181";
    this->initLimit = "5";
    this->syncLimit = "2";
    this->numServers = 0;
}

void zk_config::buildConfig(string configName)
{
    this->configName = configName;
    ofstream file;
    file.open(configName + ".cfg", ios::out);
    
    file << "tickTime=" << this->tickTime << endl
        << "dataDir=" << this->dataDir << endl
        << "clientPort=" << this->clientPort << endl
        << "initLimit=" << this->initLimit << endl
        << "syncLimit=" << this->syncLimit << endl;
    for(int i = 0; i < this->numServers; i++)
    {
        file << servers[i] << endl;
    }
    file.close();
}

void zk_config::setTickTime(string tickTime)
{
    this->tickTime = tickTime;
}

void zk_config::setDataDir(string dataDir)
{
    this->dataDir = dataDir;
}

void zk_config::setClientPort(string clientPort)
{
    this->clientPort = clientPort;
}

void zk_config::setInitLimit(string initLimit)
{
    this->initLimit = initLimit;
}

void zk_config::setSyncLimit(string syncLimit)
{
    this->syncLimit = syncLimit;
}

void zk_config::setNumServers(int numServers)
{
    this->numServers = numServers;
}

void zk_config::addNode(Server server)
{
    this->servers.push_back(server.build());
}


