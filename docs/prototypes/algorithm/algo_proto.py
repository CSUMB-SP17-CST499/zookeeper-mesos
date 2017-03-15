from task import *

f_reader = open('sample_tasks.txt','r')
f_tasks = [list(line.rstrip().split(',')) for line in f_reader.readlines()]
f_reader.close()
f_reader = open('sample_offers.txt','r')
f_offers = [line.rstrip() for line in f_reader.readlines()]
f_reader.close()

print(f_tasks)
print(f_offers)
a = Task(2,3,1,0)
b = Task(3,3,1,1)
c = Task(5,3,1,2)
d = Task(2,3,1,3)
e = Task(4,3,1,4)
f = Task(6,3,1,5)

tasks = [Task(args[0],args[1],args[2],args[3],)  for args in f_tasks]
tasks.sort(key=lambda t: t.cpu,reverse=True)
for index,task in enumerate(tasks):
	print("Task: "+str(index))
	print(task)
print(a.cpu)