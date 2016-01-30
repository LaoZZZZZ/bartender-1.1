#!/usr/bin/python

import matplotlib
matplotlib.use('Agg')

from matplotlib.backends.backend_pdf import PdfPages
import matplotlib.pyplot as plt



class pdfOutput(object):
	def __init__(self,filename):
		self.handler = PdfPages(filename)
	def write(self,data = None):
		plt.savefig(self.handler,format='pdf')
		plt.clf()
	def close(self):
		self.handler.close()


if __name__ == '__main__':
	
	x = range(10)
	y = range(20,30)
	plt.plot(x,y,'ro')
	o = pdfOutput('test.pdf')
	o.write()
	o.close()		
