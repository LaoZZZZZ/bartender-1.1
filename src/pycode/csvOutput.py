#!/usr/bin/python

class csvOutput(object):
	def __init__(self,filename):
		self.handler = open(filename,'w')
		self.start = False
	def write(self,data):
		if not self.start:
			if isinstance(data,list):
				self.handler.write(','.join(data))
			else:
				self.handler.write(data)
			
		else:
			if instance(data,list):
				self.handler.write(',')
				self.handler.write(','.join(data))
			else:
				self.handler.write(',' + data)
			
	def close(self):
		self.handler.close()


if __name__ == '__main__':
	
	o = csvOutput('test.csv')
	a = [1,2,3]
	o.write(map(str,a))
	'''
	x = range(100)
	y = range(10,110)
	plt.plot(x,y,'bx')
	o.write()
	'''
	o.close()		
