#!/usr/bin/python

from baseline import *
from benchMark import benchmark


if __name__ == '__main__':
	featurefile = 'termfreq'
	train,test = loaddata(featurefile)
	print(train,test)	
	xtrain = train['counts']
	ytrain = train['labels']
	xtest = test['counts']
	ytest = test['labels']
	data = [xtrain,ytrain,xtest,ytest]
	
