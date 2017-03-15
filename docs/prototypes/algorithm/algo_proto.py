# Task and Offers class from task.py
from task import * 
# Let's try out cmd https://docs.python.org/3.5/library/cmd.html
import cmd

# Read in tasks and turn into list of lists
# Comprehension splits line removes comma and converts to int
f_reader = open('sample_tasks.txt','r')
f_tasks = [list(map(int, line.rstrip().split(','))) for line in f_reader.readlines()]
f_reader.close()

# Same as above but with less no priority of elevation_time
f_reader = open('sample_offers.txt','r')
f_offers = [list(map(int, line.rstrip().split(','))) for line in f_reader.readlines()]
f_reader.close()

# Uncomment to see Tasks and Offers as processed files
# print(f_tasks)
# print(f_offers)

# Comprehension to create class objects
tasks = [Task(*args)  for args in f_tasks]
tasks.sort(key=lambda x: x.cpu,reverse=True)

# Same as above with less parameters
offers = [Offer(*args)  for args in f_offers]
offers.sort(key=lambda x: x.cpu,reverse=True)

#Based off of example from python docs
class SchedulerShell(cmd.Cmd):
	intro = 'Welcome to the example scheduling algorithm.   Type help or ? to list commands.\n'
	prompt = '> '
	
	def do_quit(self, arg):
		'Quits the example.'
		raise SystemExit
	
	def do_tasks(self, arg):
		'Lists tasks held in memory.'
		for index,task in enumerate(tasks):
			print("Task: "+str(index))
			print(task)
	
	def do_offers(self, arg):
		'Lists offers held in memory.'
		for index,offer in enumerate(offers):
			print("Offer: "+str(index))
			print(offer)
	
	def do_add_task(self,arg):
		'Adds task specified by you. cpu,memory,priority,elevation_time: add_task 6 3 1 5'
		tasks.append(Task(*parse(arg)))
		tasks.sort(key=lambda x: x.cpu,reverse=True)
		
	def do_rm_task(self,arg):
		'Removes task specified by you. Call by task id: rm_task 0'
		del tasks[parse(arg)[0]]
	
	def do_add_offer(self,arg):
		'Adds offer specified by you. cpu,memory: add_offer 7 2'
		offers.append(Offer(*parse(arg)))
		offers.sort(key=lambda x: x.cpu,reverse=True)
		
	def do_rm_offer(self,arg):
		'Removes offer specified by you. Call by offer id: rm_offer 0'
		del offers[parse(arg)[0]]
		
	def do_sl_cpu(self,arg):
		'Solves task - offer resolution based on cpu'
		skip = 0
		matches = []
		for x in range(0,len(tasks)):
			if(len(matches) == len(tasks)):
					break
			for y in range(skip,len(offers)):
				if(len(matches) == len(tasks)):
					break
				if(tasks[x].cpu <= offers[y].cpu):
					skip += 1
					matches.append(Match(tasks[x],offers[y]))
					break
		for index,match in enumerate(matches):
			print("Match: "+str(index))
			print(match)
			tasks.remove(match.task)
			offers.remove(match.offer)
		
def parse(arg):
	'Convert a series of zero or more numbers to an argument list'
	return list(map(int, arg.split()))

if __name__ == '__main__':
	SchedulerShell().cmdloop()

