#!/usr/bin/python

import os
import sys

def checkUnique(f):
	print(f)
	table = {}
	for line in open(f,'r'):
		key,value = line.rstrip().split()
		if key in table:
			print(key, table[key])
			return False	
		else:
			table[key] = value
	print(len(table))
	return True

if __name__ == '__main__':
	f = '../simulation_data/True_barcode_freq.txt'
	print(checkUnique(f))
		

