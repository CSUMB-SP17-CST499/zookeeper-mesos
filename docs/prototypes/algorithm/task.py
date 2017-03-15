import json

class Task:
	def __init__(self, cpu, mem, prio, elev):
		self.cpu = cpu
		self.mem = mem
		self.prio = prio
		self.elev = elev
		
	def __str__(self):
		return "{\n\t"+"'cpu': "+str(self.cpu)+",\n\t"+"'memory': "+str(self.mem)+",\n\t"+"'priority': "+str(self.prio)+",\n\t"+"'elevation_time': "+str(self.elev)+"\n}"

class Offer:
	def __init__(self, cpu, mem):
		self.cpu = cpu
		self.mem = mem
		
	def __str__(self):
		return "{\n\t"+"'cpu': "+str(self.cpu)+",\n\t"+"'memory': "+str(self.mem)+"\n}"
		
class Match:
	def __init__(self, task, offer):
		self.task = task
		self.offer = offer
		
	def __str__(self):
		return ("    Task    |    Offer    \n"+
				"--------------------------\n"+
				" cpu   |  "+str(self.task.cpu)+"  |  "+str(self.offer.cpu)+"  |   cpu \n"
				+" mem   |  "+str(self.task.mem)+"  |  "+str(self.offer.mem)+"  |   mem \n"
				+" prio  |  "+str(self.task.prio)+"  |     |  prio \n"
				+" elev  |  "+str(self.task.elev)+"  |     |  elev \n")