#!/usr/bin/python

if __name__ == '__main__':
	import os
	import sys
	prefix = '/home/laozzzzz/Documents/barcode_single_end/BarcodeSimulation/result'
	time_points = range(1, 162, 8)
	input_files = [(prefix + '_' + str(t) + '_cluster.csv', prefix + '_' + str(t) + '_bp_frequency.csv') for t in time_points]
	serialized = ''
	for k,v in input_files:
		serialized += k + ' ' + v + ' '
	print(serialized.strip())
	
