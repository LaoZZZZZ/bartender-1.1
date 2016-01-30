#!/usr/bin/python

import sys
import os
import numpy as np
from scipy import io
from grid_search_cv import *
import glob
import multiprocessing
from outPutMedia import outPutMedia
def ourPreprocess(featurefile,originalfeatures,outprefix):
	selectedFeatures = io.loadmat(featurefile)
	keys = selectedFeatures.keys()
	print(keys)
	assert(len(keys) >= 2)
	index = None
	trainfeatures = None
	testfeatures = None
	for k in keys:	
		if 'feature' in k:
			index = selectedFeatures[k][0,:]-1
		elif 'test' in k :
			testfeatures = selectedFeatures[k]
		elif 'train' in k:
			trainfeatures = selectedFeatures[k]
	
	assert(None != index)
	assert(None != trainfeatures)
	assert(None != testfeatures)
	trainoriginal = io.loadmat(originalfeatures+'_train.mat')

	test_original = io.loadmat(originalfeatures+'_test.mat')
	# feature names
	feature_names = trainoriginal['feature_names'][index]
	trainoriginal['counts'] = trainfeatures
	trainoriginal['feature_names'] = feature_names
	#data = {'counts':features,'labels':original['labels'],'feature_names':feature_names,'category':original['category']}
	
	io.savemat(outprefix+'_train.mat',trainoriginal)
	# apply the same feature selection to the test dataset
	#test_count = test_original['counts'][:,index]		
	test_original['counts'] = testfeatures
	test_original['feature_names'] = feature_names
	io.savemat(outprefix+'_test.mat',test_original)	
	return outprefix
def drive(feature,featureNumb,outprefix,method):
	print(feature)
	featureSelected = [1,5,10,15,20,30,40,80,100]	
	if featureNumb > 100:
		featureSelected += range(120,featureNumb+1,40)
	result = []
	for f in featureSelected:
		result.append(method(feature,f))
	cont = '\n'.join([str(k) + ',' + str(v) for k, v in zip(featureSelected,result)])
	if 'svm' in str(method):
		open(outprefix + '_svmTrain'+ '.csv','w').write(cont)
	elif 'NB' in str(method):
		open(outprefix + '_NBTrain'+ '.csv','w').write(cont)
	elif 'knn' in str(method):
		open(outprefix + '_knnTrain'+ '.csv','w').write(cont)
	
		
if __name__ == '__main__':
	original = '../features/bagofword'
	directory = './selectedFeatures'
	feature = sys.argv[1] 
	featureNumb = int(sys.argv[2]) 
	fold = int(sys.argv[3])
	outprefix = os.path.splitext(feature)[0]
	feature = ourPreprocess(feature,original,outprefix)
	method = [svmTrain,NBTrain,knnTrain]
	process = []
	for m in method:
		temp = multiprocessing.Process(target=drive,args = (feature,featureNumb,outprefix,m))
		process.append(temp)
		temp.start()
	for p in process:
		p.join()

	
