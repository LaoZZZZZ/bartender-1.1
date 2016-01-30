#!/usr/bin/python

import sys
import os
import glob
import numpy as np
def drive_single(featurefile):
	#for f in glob.glob(directory + '/FS*.mat'):
	#f = os.path.join(directory,'FS_GREEDY_DF.mat')
	f = featurefile
	featurenumb = 200
	fold = '10'
	#for i in np.arange(1,200,step = 10):
	logfile = '../predict_result/' + os.path.splitext(os.path.basename(f))[0] + '_' + str(featurenumb) +'_result'
	cmd = 'nohup ./our_algorithm.py ' + f +' ' + str(featurenumb) + ' ' + fold + ' > ' + logfile + ' 2>&1 &'	
	print(cmd)
	os.system(cmd)	
def drive_single_original(featurefile):
	#for f in glob.glob(directory + '/FS*.mat'):
	#f = os.path.join(directory,'FS_GREEDY_DF.mat')
	f = featurefile
	featurenumb = 600 
	fold = '10'
	#for i in np.arange(1,200,step = 10):
	logfile = '../predict_result/' + os.path.splitext(os.path.basename(f))[0] + '_' + str(featurenumb) +'_result'
	cmd = 'nohup ./original_drive.py ' + f +' ' + str(featurenumb) + ' ' + fold + ' > ' + logfile + ' 2>&1 &'	
	print(cmd)
	os.system(cmd)
if __name__ == '__main__':
	directory = '../selectedFeatures'
	#drive_single(os.path.join(directory,'FS_GREEDY_DF.mat'))
	drive_single(os.path.join(directory,'FS_SIMANN_MI_REP_1.mat'))
	'''
	for f in glob.glob(os.path.join(directory,'FS_SI*.mat')):
		print(f)
		drive_single(f)
	directory = '../features'
	feature = os.path.join(directory,'bagofword')
	drive_single_original(feature)
	feature = os.path.join(directory,'termfreqinverse')
	drive_single_original(feature)
	feature = os.path.join(directory,'termfreq')
	drive_single_original(feature)
	'''
