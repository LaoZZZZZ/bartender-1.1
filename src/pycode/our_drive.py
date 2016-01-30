#!/usr/bin/python

import sys
import os
import numpy as np
from scipy import io
from gride_search_cv import *



def ourPreprocess(featurefile,transformer,originalfeatures,outprefix):
	selectedFeatures = io.loadmat(featurefile)
	keys = selectedFeatures.keys()
	print(keys)
	assert(len(keys) >= 2)
	index = None
	features = None
	for k in keys:	
		if 'feature' in k:
			index = selectedFeatures[k][0,:]-1
		elif 'counts' in k :
			features = selectedFeatures[k]
	assert(None != index)
	assert(None != features)
	original = io.loadmat(originalfeatures+'_train.mat')
	test_original = io.loadmat(originalfeatures+'_test.mat')
	# feature names
		
	feature_names = original['feature_names'][transformer[index]]
	print(feature_names)
	original['counts'] = features
	original['feature_names'] = feature_names
	#data = {'counts':features,'labels':original['labels'],'feature_names':feature_names,'category':original['category']}
	
	io.savemat(outprefix+'_train.mat',original)
	# apply the same feature selection to the test dataset
	test_count = test_original['counts'][:,transformer[index]]		
	test_original['counts'] = test_count
	test_original['feature_names'] = feature_names
	io.savemat(outprefix+'_test.mat',test_original)	
	return outprefix
if __name__ == '__main__':
	import sys
	import glob
	import os
	original = './data/bagofword'
	directory = './data'
	feature = sys.argv[1] 
	featureNumb = sys.argv[2] 
	transformFile = sys.argv[3]
	outprefix = os.path.splitext(feature)[0]
	transformer = io.loadmat(transformFile)['indx_cut'][0,:] - 1
	feature = ourPreprocess(feature,transformer,original,outprefix)
	'''
	print(feature,outprefix)	
	svmTrain(feature,featureNumb)
	NBTrain(feature,featureNumb)
	knnTrain(feature,featureNumb)
