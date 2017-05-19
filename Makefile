test_framework:	test_framework.cpp
	g++ -Wall -g -pthread -std=c++11 -I/usr/include/ -I/usr/include/mesos/ -I/home/mekeeper/zk/extern/mesos-1.2.0/src/ -I/home/mekeeper/zk/extern/mesos-1.20/src/ -I/usr/local/lib -I/usr/local/lib/ -o test_framework_copy test_framework_copy.cpp -L -pthread -lprotobuf -lglog -lmesos -lpthread -lboost_thread
clean:		
	rm test_framework
