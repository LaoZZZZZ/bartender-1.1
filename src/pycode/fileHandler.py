#!/usr/bin/python

from pdfOutput import pdfOutput
from pngOutput import pngOutput
from csvOutput import csvOutput
class outFileHandler(object):
	def __init__(self,filename):
		prefix,ext = filename.split('.')
		if ext == 'txt':
			self.handler = open(filename,mode)
		elif ext == 'pdf':
			self.handler = pdfOutput(filename)
		elif ext == 'png':
			self.handler = pngOutput(filename)
		elif ext == 'csv':
			self.handler = csvOutput(filename)
		else:
			print('current does not support file format %s'%(ext))

	def write(self,data):
		self.handler.write(data)
	def close(self):
		self.handler.close()

if __name__ == '__main__':
	o = outFilehandler('output.txt')
	o.write('hello,world')
	o.close()	
