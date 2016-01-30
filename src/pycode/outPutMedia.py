#!/usr/bin/python

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from fileHandler import outFileHandler
class outPutMedia(object):
	def __init__(self,filename):
		self.filename = filename
		self.handler = outFileHandler(filename)
		self.open = True
	def write(self,data):
		if self.open:
			self.handler.write(data)
		else:
			raise IOError('trying to write data to already closed file %s'%(self.filename))
	def close(self):
		if self.open:
			self.handler.close()
			self.open = False
	def __del__(self):
		self.close()
		self.open = False


if __name__ == '__main__':
	filename = 'test.pdf'
	o = outPutMedia(filename)
	plt.plot(range(10),range(10,20),'ro')
	o.write(None)
	o.close()
	
	
		
