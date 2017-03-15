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
# Would be nice to assign parameters better
tasks = [Task(args[0],args[1],args[2],args[3])  for args in f_tasks]
tasks.sort(key=lambda t: t.cpu,reverse=True)

# Same as above with less parameters
offers = [Offer(args[0],args[1])  for args in f_offers]
offers.sort(key=lambda t: t.cpu,reverse=True)

#Based off of example from python docs
class SchedulerShell(cmd.Cmd):
	intro = 'Welcome to the example scheduling algorithm.   Type help or ? to list commands.\n'
	prompt = '> '
	
	def do_quit(self, args):
		'Quits the example.'
		raise SystemExit
	
	def do_tasks(self, args):
		'Lists tasks held in memory.'
		for index,task in enumerate(tasks):
			print("Task: "+str(index))
			print(task)
	
	def do_offers(self, args):
		'Lists offers held in memory.'
		for index,offer in enumerate(offers):
			print("Offer: "+str(index))
			print(offer)
	
def i_parser(args):
	'Convert a series of zero or more numbers to an argument tuple'
	return tuple(map(int, arg.split()))

if __name__ == '__main__':
	SchedulerShell().cmdloop()

