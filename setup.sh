#!/bin/bash

# copy current zk config to new mesos_zk dir
#
# /usr/share/zookeeper -> /usr/share/mesos_zk
# first must delete any current /usr/share/mesos_zk dir, then recreate and copy files

mesos_zk_dir="/usr/share/mesos_zk"
zookeeper_dir="/usr/share/zookeeper"


if [ -d "$mesos_zk_dir" ]; then
	rm -rf $mesos_zk_dir
fi

mkdir $mesos_zk_dir
cp -R $zookeeper_dir/. $mesos_zk_dir

# create conf/zoo.cfg

mkdir $mesos_zk_dir/conf
touch $mesos_zk_dir/conf/zoo.cfg

# populate zoo.cfg

conf_dir=$mesos_zk_dir/conf/zoo.cfg

echo "%s" >> $conf_dir

# start zk
./$mesos_zk_dir/bin/zkServer.sh start
