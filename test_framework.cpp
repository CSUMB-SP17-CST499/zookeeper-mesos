// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

#include <mesos/resources.hpp>
#include <mesos/scheduler.hpp>
#include <mesos/type_utils.hpp>
#include <stout/check.hpp>
#include <stout/exit.hpp>
#include <stout/flags.hpp>
#include <stout/numify.hpp>
#include <stout/option.hpp>
#include <stout/os.hpp>
#include <stout/path.hpp>
#include <stout/stringify.hpp>

#include "logging/flags.hpp"
#include "logging/logging.hpp"

#include <fstream>
#include <streambuf>

using namespace mesos;

using boost::lexical_cast;

using std::cerr;
using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::vector;
using std::map; 

using mesos::Resources;

const int32_t CPUS_PER_TASK = 1;
const int32_t MEM_PER_TASK = 128;
int NODES = 2;
int NUM_MASTERS = 1;
int NUM_AGENTS = 1;

//static 

class TestScheduler : public Scheduler
{
public:
  TestScheduler(
      bool _implicitAcknowledgements,
      const ExecutorInfo& _executor,
      const string& _role)
    : implicitAcknowledgements(_implicitAcknowledgements),
      executor(_executor),
      role(_role),
      tasksLaunched(0),
      tasksFinished(0),
      totalTasks(1){}

  virtual ~TestScheduler() {}

  virtual void registered(SchedulerDriver*,
                          const FrameworkID&,
                          const MasterInfo&)
  {
    cout << "Registered!" << endl;
  }

  virtual void reregistered(SchedulerDriver*, const MasterInfo& masterInfo) {}

  virtual void disconnected(SchedulerDriver* driver) {}

  virtual void resourceOffers(SchedulerDriver* driver,
                              const vector<Offer>& offers){
    if(NODES == 0)
	{
		cout << "No nodes needed..\n";
		return;
	}

    map<string,Offer> zkOffers;   
    vector<string> agents;
    vector<string> masters;

    while(NODES > 0 ){

      foreach(const Offer& offer, offers){
	cout << "Received offer " << offer.id() << " with " << offer.resources() << " on " << offer.hostname()<< endl;
	Resources taskResources = Resources::parse("cpus:" + stringify(CPUS_PER_TASK) +                                         
						   ";mem:" + stringify(MEM_PER_TASK)).get();
	taskResources.allocate(role);
	Resources offerResource = offer.resources(); 
	if(offerResource.flatten().contains(taskResources)){
	  if(zkOffers.count(offer.hostname()) == 0 && NODES > 0)
	  {
		zkOffers[offer.hostname()] = offer;
		NODES --; 
		if(NUM_MASTERS > 0)
		{
			NUM_MASTERS--;
			masters.push_back(offer.hostname());
		}
		if(NUM_AGENTS > 0 && NUM_MASTERS == 0)
		{
			NUM_AGENTS--;
			agents.push_back(offer.hostname());
		}	
	  }
	}
	}
	
     }
      for (auto const& x : zkOffers){
	int taskId = tasksLaunched++;
	cout << "\nLaunching task " << taskId << " using offer "<< x.second.id() << endl;
	std::string command = "";
	// if offer is for a master
	generate_conf(masters);
	if(std::find(masters.begin(), masters.end(), x.second.hostname()) != masters.end())
	{
		command = generate_master(agents, masters, x.second.hostname());

	}

	// if offer is for agent
	else
	{
		command = generate_agent(masters, x.second.hostname());

	}
	vector<TaskInfo> tasks;
	TaskInfo task;
	task.set_name("Task " + lexical_cast<string>(taskId));
	task.mutable_task_id()->set_value(lexical_cast<string>(taskId));
	task.mutable_slave_id()->MergeFrom(x.second.slave_id());
	task.mutable_command()->set_value(command);


	Resources taskResources = Resources::parse("cpus:" + stringify(CPUS_PER_TASK) +                                         
						   ";mem:" + stringify(MEM_PER_TASK)).get();
	taskResources.allocate(role);
	Try<Resources> flattened = taskResources.flatten(role);
	Resources offerResource = x.second.resources(); 
	Option<Resources> resources = offerResource.find(flattened.get());
	task.mutable_resources()->MergeFrom(resources.get());



	tasks.push_back(task);
	driver->launchTasks(x.second.id(), tasks);
	

    }
	cout << "after launch\n";
   
  }
 
  // generating master command
  std::string generate_master(vector<string> agents, vector<string> masters, string self)
  {
	std::ifstream f("setup.sh");
	std::string setup((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());	

	// add dataDir to setup
	// first must find from conf
	string conf = generate_conf(masters);
	// find dataDir line
	string dataDir_line;
	std::istringstream line(conf);
	string dataDir;
	while(std::getline(line, dataDir_line))
	{
		if(dataDir_line.find("dataDir") != std::string::npos)
		{
			dataDir = dataDir_line.substr(dataDir_line.find("dataDir")+8);
		}
	}
	
	// add dataDir to setup.sh
	setup.replace(setup.find("%d"), string("%d").length(), dataDir);
		
	// add myid to setup.sh
	// find self hostname -> add vector index to myid
	int id = find(masters.begin(), masters.end(), self) - masters.begin();
	string create_command = "touch $dataDir/myid;\n";
	string echo_command = "echo " + std::to_string(id+1) + " > $dataDir/myid;\n";
	setup.replace(setup.find("%t"), string("%t").length(), string(create_command + echo_command));
	

	// add conf to setup script
	setup.replace(setup.find("%s"), string("%s").length(), conf);
	cout << setup << endl;
	return "touch /home/mekeeper/master";

  }
  
  // generating agent command
  std::string generate_agent(vector<string> masters, string self)
  {

	std::ifstream f("setup.sh");
	std::string setup((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());	
	
	string conf = generate_conf(masters);
	string dataDir_line;
	std::istringstream line(conf);
	string dataDir;
	while(std::getline(line, dataDir_line))
	{
		if(dataDir_line.find("dataDir") != std::string::npos)
		{
			dataDir = dataDir_line.substr(dataDir_line.find("dataDir")+8);
		}
	}
	
	// add dataDir to setup.sh
	setup.replace(setup.find("%d"), string("%d").length(), dataDir);

	// add conf to setup script
	setup.replace(setup.find("%s"), string("%s").length(), conf);
	cout << setup << endl;

	// remove myid lines
	setup.erase(setup.find("%t"), string("%t").length());
	cout << setup << endl;
	return "touch /home/mekeeper/agent";

  }

  // generate conf file
  std::string generate_conf(vector<string> masters)
  {
	// read conf to string
	std::ifstream f("mesos_zk.cfg");
	std::string conf((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	string ports = ":4888:5888";
	for(unsigned i = 0; i < masters.size(); i++)
	{
		conf += string("server.") + std::to_string(i+1) + string(".") + masters[i] + ports + "\n";
	}
	return conf;
  }

  virtual void offerRescinded(SchedulerDriver* driver,
                              const OfferID& offerId) {}

  virtual void statusUpdate(SchedulerDriver* driver, const TaskStatus& status)
  {
    int taskId = lexical_cast<int>(status.task_id().value());

    cout << "Task " << taskId << " is in state " << status.state() << endl;

    if (status.state() == TASK_FINISHED) {
      tasksFinished++;
    }

    if (status.state() == TASK_LOST ||
        status.state() == TASK_KILLED ||
        status.state() == TASK_FAILED ||
	status.state() == TASK_ERROR) {
      cout << "Aborting because task " << taskId
           << " is in unexpected state " << status.state()
           << " with reason " << status.reason()
           << " from source " << status.source()
           << " with message '" << status.message() << "'"
           << endl;

      driver->abort();
    }

    if (!implicitAcknowledgements) {
      driver->acknowledgeStatusUpdate(status);
    }

    if (tasksFinished == totalTasks) {
      driver->stop();
    }
  }

  virtual void frameworkMessage(SchedulerDriver* driver,
                                const ExecutorID& executorId,
                                const SlaveID& slaveId,
                                const string& data) {}

  virtual void slaveLost(SchedulerDriver* driver, const SlaveID& sid) {}

  virtual void executorLost(SchedulerDriver* driver,
                            const ExecutorID& executorID,
                            const SlaveID& slaveID,
                            int status) {}

  virtual void error(SchedulerDriver* driver, const string& message)
  {
    cout << message << endl;
  }

private:
  const bool implicitAcknowledgements;
  const ExecutorInfo executor;
  string role;
  int tasksLaunched;
  int tasksFinished;
  int totalTasks;
};


void usage(const char* argv0, const flags::FlagsBase& flags)
{
  cerr << "Usage: " << Path(argv0).basename() << " [...]" << endl
       << endl
       << "Supported options:" << endl
       << flags.usage();
}


class Flags : public virtual mesos::internal::logging::Flags
{
public:
  Flags()
  {
    add(&Flags::role, "role", "Role to use when registering", "*");
    add(&Flags::master, "master", "ip:port of master to connect");
  }

  string role;
  Option<string> master;
};


int main(int argc, char** argv)
{
  // Find this executable's directory to locate executor.
  Option<string> value = os::getenv("MESOS_HELPER_DIR");

  Flags flags;

  Try<flags::Warnings> load = flags.load(None(), argc, argv);

  if (load.isError()) {
    cerr << load.error() << endl;
    usage(argv[0], flags);
    exit(EXIT_FAILURE);
  } else if (flags.master.isNone()) {
    cerr << "Missing --master" << endl;
    usage(argv[0], flags);
    exit(EXIT_FAILURE);
  }

  internal::logging::initialize(argv[0], flags, true); // Catch signals.

  // Log any flag warnings (after logging is initialized).
  foreach (const flags::Warning& warning, load->warnings) {
    LOG(WARNING) << warning.message;
  }

  ExecutorInfo executor;
  executor.mutable_executor_id()->set_value("default");
  executor.mutable_command()->set_value("touch ~/zkTest.txt");

  FrameworkInfo framework;
  framework.set_user(""); // Have Mesos fill in the current user.
  framework.set_name("zk test");
  framework.set_role(flags.role);

  value = os::getenv("MESOS_CHECKPOINT");
  if (value.isSome()) {
    framework.set_checkpoint(
        numify<bool>(value.get()).get());
  }

  bool implicitAcknowledgements = true;
  if (os::getenv("MESOS_EXPLICIT_ACKNOWLEDGEMENTS").isSome()) {
    cout << "Enabling explicit acknowledgements for status updates" << endl;

    implicitAcknowledgements = false;
  }

  MesosSchedulerDriver* driver;
  TestScheduler scheduler(implicitAcknowledgements, executor, flags.role);

  if (os::getenv("MESOS_AUTHENTICATE_FRAMEWORKS").isSome()) {
    cout << "Enabling authentication for the framework" << endl;

    value = os::getenv("DEFAULT_PRINCIPAL");
    if (value.isNone()) {
      EXIT(EXIT_FAILURE)
        << "Expecting authentication principal in the environment";
    }

    Credential credential;
    credential.set_principal(value.get());
    framework.set_principal(value.get());

    value = os::getenv("DEFAULT_SECRET");
    if (value.isNone()) {
      EXIT(EXIT_FAILURE)
        << "Expecting authentication secret in the environment";
    }

    credential.set_secret(value.get());

    driver = new MesosSchedulerDriver(
        &scheduler,
        framework,
        flags.master.get(),
        implicitAcknowledgements,
        credential);
  } else {
    framework.set_principal("");

    driver = new MesosSchedulerDriver(
        &scheduler,
        framework,
        flags.master.get(),
        implicitAcknowledgements);
  }

  int status = driver->run() == DRIVER_STOPPED ? 0 : 1;

  // Ensure that the driver process terminates.
  driver->stop();

  delete driver;
  return status;
}
