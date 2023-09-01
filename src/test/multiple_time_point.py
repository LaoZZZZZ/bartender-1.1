#!/usr/bin/env python

import os
import sys
import glob
import re

from multiprocessing import Process

def run_each(f):
	prefix = f.split('.')[0]
	time_point = re.findall('\d+',os.path.basename(f))[0]	
	command = "./simulation_test " + f + ' /home/laozzzzz/Documents/barcode_single_end/BarcodeSimulation/result_' + time_point  + ' 10 0'
	print(command)
	os.system(command)
if __name__ == '__main__':
	timepoints = []
        jobs = []
	for f in glob.glob("/home/laozzzzz/Documents/barcode_single_end/BarcodeSimulation/Sequenced_time_point_*.csv"):
		p = Process(target = run_each, args = (f,))
		p.start()
		jobs.append(p)
		if len(jobs) > 4:
			for p in jobs:
				p.join()		
			jobs = []
	for f in jobs:
		p.join()
	



