g++ -Wall -g -O2 -pthread -std=c++11 -I/home/mekeeper/zk/extern/mesos-1.2.0/3rdparty/boost-1.53.0/boost -I/home/mekeeper/zk/extern/mesos-1.2.0/3rdparty/zookeeper-3.4.8/src -I/usr/local/lib -I/usr/local/include/ -I/usr/local/include/google/protobuf -I/home/mekeeper/zk/extern/mesos-1.2.0/src -I/usr/local/lib -L -lprotobuf -lmesos -lglog $1 -o compiled-$1 -I/usr/local/lib -lmesos -lglog -lprotobuf 