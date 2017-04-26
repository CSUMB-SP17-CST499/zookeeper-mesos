#/bin/bash

# create data directory and remove if exists

zk_home="/user/share/mesos_zk/"
zk_dependencies="/usr/share/zookeeper/"

#env variables
dataDir="/var/lib/mesos_zk/"
tickTime="2000"
clientPort="2181"
initLimit="5"
syncLimit="2"
dataLogDir="/var/log/mesos_zk/"
nodePortFollowers="2888"
nodePortLeader="3888"

myId="2"
numNodes="2"
declare -a nodeIds=("server.1=IPADDRESS1" "server.2=IPADDRESS2")
if [ -d "$dataDir" ]
then
	rm $dataDir
fi

if [ -d "$dataLogDir" ]
then
	rm $dataLogDir
fi

if [ -d "$zk_home" ]
then
	rm $zk_home
fi

if [ -d "/usr/share/mesos_zk/" ]
then
	rm "/usr/share/mesos_zk/"
fi


mkdir $dataDir
mkdir $dataLogDir
mkdir $zk_home
mkdir "/usr/share/mesos_zk/"



#copy current zk dir to ours

cp -R "$zk_dependencies. $zk_home"
cp -R "/var/lib/zookeeper/. /var/lib/mesos_zk"
cp -R "/var/log/zookeeper/. /var/log/mesos_zk"

read -d conf <<- EOF
	dataDir=$dataDir
	tickTime=$tickTIme
	clientPort=$clientPort
	initLimit=$initLimit
	syncLimit=$syncLimit
	dataLogDir=$dataLogDir
EOF

mkdir "/usr/share/mesos_zk/bin"
echo "$conf" > "/usr/share/mesos_zk/bin/zoo.cfg"


for i in $(seq "$numNodes")
do
	echo "$nodeIds[$i]$:$nodePortFollowers:$nodePortLeaders" >> "/usr/share/mesos_zk/bin/zoo.cfg"
done
