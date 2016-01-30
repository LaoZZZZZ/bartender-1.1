#!/usr/bin/python

import os
import sys
import glob

def collectSource(path):
	target = os.path.join(path,'*.cpp')
	res = []
	for f in glob.glob(target):
		os.system('cp ' + f + ' src/' + os.path.basename(f))
		res.append(os.path.basename(f))
	header =os.path.join(path,'*.h*')
	for f in glob.glob(header):
		os.system('cp ' + f + ' src/' + os.path.basename(f))	 
	return res 
def generateObj(files,prefix,suffix):
	res =[]
	for f in files:
		name = os.path.basename(os.path.splitext(f)[0])
		res.append(os.path.join(prefix,name)+suffix)
	return res
def generateMakefile(path,platform,home):
	generateDirectory(home)	
 	res= collectSource(path)
	print(res)
	#obj = ' '.join(generateObj(res,'./src','.o'))
	source = ' '.join(generateObj(res,'./src','.cpp'))
	platform = open(platform,'r').read()
	hand = open(os.path.join(home,'Makefile'),'w')
	hand.write(platform)
	hand.write('SOURCES\t= ' +source + '\n')
	#hand.write('OBJECTS\t= ' + obj + '\n')
	cont = open('makeOptions','r').read()
	hand.write(cont)	
	#hand.write(makedepend())
	#hand.close()
	#os.system('make depend')
def makedepend():
	dflags = ' -O -DHACK -I ./inc -Y'
	comd = 'DFLAGS\t= ' + dflags + '\n' + 'depend:\n\t'+ 'makedepend  ' + '$(DFLAGS)  $(SRCS)\n'
	return comd 
	
def generateDirectory(home):
	if not os.path.isdir(os.path.join(home,'inc')):
		os.mkdir(os.path.join(home,'inc'))
	if not os.path.isdir(os.path.join(home,'src')):
		os.mkdir(os.path.join(home,'src'))
	if not os.path.isdir(os.path.join(home,'obj')):
		os.mkdir(os.path.join(home,'obj'))
	if not os.path.isdir(os.path.join(home,'bin')):
		os.mkdir(os.path.join(home,'bin'))
if __name__ == '__main__':
	#path = '../barcode_single_end/barcode_analysis'
	path = '/Users/lukez/Documents/barcode_project/'
	generateMakefile(path,'platform','./')


