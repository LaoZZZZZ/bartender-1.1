#!/usr/bin/python

import matplotlib
matplotlib.use('Agg')

from matplotlib.backends.backend_pdf import PdfPages
import matplotlib.pyplot as plt



class pngOutput(object):
	def __init__(self,filename):
		self.filename = filename
	def write(self,data = None):
		plt.savefig(self.filename)
	def close(self):
		pass


if __name__ == '__main__':
	
	o = pngOutput('test.png')
	x = range(10)
	y = range(20,30)
	plt.plot(x,y,'ro')

	o.write()
	'''
	x = range(100)
	y = range(10,110)
	plt.plot(x,y,'bx')
	o.write()
	'''
	o.close()		
