#!/usr/bin/python
from scipy import io
import numpy as np
from baseline import loaddata,featureselection
from grid_search_cv import *
from our_algorithm import drive
import multiprocessing
# take in original data and select the features
def preprocess(prefix,totalfeatures):
	train,test = loaddata(prefix)
	X_train,X_test,feature_names=featureselection((train,test),totalfeatures)
	trainoutput = prefix + '_chi_train.mat'
	testoutput = prefix + '_chi_test.mat'
	
	train['counts'] = X_train
	train['feature_names'] = feature_names
	io.savemat(trainoutput,train)
	test['counts'] = X_test
	test['feature_names'] = feature_names
	print(feature_names)
	io.savemat(testoutput,test)
	return prefix+'_chi'	
if __name__ == '__main__':
	import sys
	import glob
	import os
	original = sys.argv[1]
	featureNumb = int(sys.argv[2])
	fold = int(sys.argv[3])
	print(original,featureNumb,fold)
	feature = preprocess(original,featureNumb)
	outprefix = os.path.splitext(feature)[0]
	print(outprefix)
	method = [svmTrain,NBTrain,knnTrain]
	process = []
	for m in method:
		temp = multiprocessing.Process(target = drive,args = (feature,featureNumb,outprefix,m))
		temp.start()
		process.append(temp)
	for p in process:
		p.join()
