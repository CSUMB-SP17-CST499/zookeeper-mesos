#!/bin/bash

# copy current zk config to new mesos_zk dir;
#;
# /usr/share/zookeeper -> /usr/share/mesos_zk;
# first must delete any current /usr/share/mesos_zk dir, then recreate and copy files;

log="/home/mekeeper/log";
touch $log;
echo "created log" > $log;


mesos_zk_dir="/usr/share/mesos_zk";
zookeeper_dir="/usr/share/zookeeper";
dataDir="%d";

echo "created vars" >> $log;

if [ -d "$mesos_zk_dir" ]; then
	rm -rf $mesos_zk_dir;
fi;

echo "checked mesos_zk_dir" >> $log;

if [ -d "$dataDir" ]; then
	rm -rf $dataDir;
fi;

echo "checked dataDir" >> $log;

mkdir $dataDir;

echo "created dataDir" >> $log;

mkdir $mesos_zk_dir;

echo "created zk_dir" >> $log;

cp -R $zookeeper_dir/. $mesos_zk_dir;

echo "copied data" >> $log;

# create conf/zoo.cfg;

mkdir $mesos_zk_dir/conf;

echo "created conf dir" >> $log;

touch $mesos_zk_dir/conf/zoo.cfg;

echo "created conf file" >> $log;
# populate zoo.cfg;

conf_dir=$mesos_zk_dir/conf/zoo.cfg;

echo "created conf_dir var" >> $log;

echo "%s" >> $conf_dir;

echo "echod to conf file" >> $log;

%t

# start zk;
sh $mesos_zk_dir/bin/zkServer.sh start $conf_dir;

echo "started zk" >> $log;
