#!/usr/bin/python

import numpy as np
from baseline import loaddata
from grid_search_cv import *
from seminb import SemiNB
from scipy.sparse import vstack

def dataTransformation(data,label):
	td = np.transpose(data)
	samplesize = data.shape[0]
	category = np.unique(label)
	delta = np.zeros((samplesize,len(category)))
	for i in range(len(label)):
		delta[i][label[i]] = 1
	return (td.todense(),delta)
		
def splitDataByClass(data,label,percentage = 0.3):
	category = np.unique(label)
	labeled = None
	unlabeled = None
	y_labeled = None
	y_unlabeled = None
	first = False	
	for c in category:
		split = np.nonzero(label == c)[0]
		sz = int(percentage*len(split))	
		choice = np.random.choice(split,sz,replace=False)
		remaining = np.setdiff1d(split,choice)
#		print(choice.shape,remaining.shape,split.shape)
		if first:
			#labeled = np.concatenate((labeled,data[choice,:]),axis = 0)
			labeled = vstack((labeled,data[choice,:]))
			unlabeled =vstack((unlabeled,data[remaining,:]))
			y_labeled = np.concatenate((y_labeled,label[choice]))
			y_unlabeled = np.concatenate((y_unlabeled,label[remaining]))
		else:
			labeled = data[choice,:]
			unlabeled = data[remaining,:]	
			y_labeled = label[choice]
			y_unlabeled = label[remaining]			
			first=True	
	return ((labeled,y_labeled),(unlabeled,y_unlabeled))	
		
if __name__ == '__main__':
	np.random.seed(511)
	snb = SemiNB()
	prefix = '../features/bagofword'
	data = loaddata(prefix)
	print(data[0]['counts'].shape,data[1]['counts'].shape)
	labeled,unlabeled = splitDataByClass(data[0]['counts'],data[0]['labels'][0,:],0.5)
	td,delta =dataTransformation(labeled[0],labeled[1])
	print(td.shape,delta.shape)	 
	snb.train(td,delta)
	test_td,test_delta = dataTransformation(data[1]['counts'],data[1]['labels'])
	print(test_td.shape)
	result = snb.predict_all(np.transpose(test_td)	)
	print(result)
		
