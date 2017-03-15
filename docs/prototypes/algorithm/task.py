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