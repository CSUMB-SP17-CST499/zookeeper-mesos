from task import *

a = Task(2,3,1,0)
b = Task(3,3,1,1)
c = Task(5,3,1,2)
d = Task(2,3,1,3)
e = Task(4,3,1,4)
f = Task(6,3,1,5)

tasks = [a,b,c,d,e,f]
tasks.sort(key=lambda t: t.cpu,reverse=True)
for index,task in enumerate(tasks):
	print("Task: "+str(index))
	print(task)
print(a.cpu)