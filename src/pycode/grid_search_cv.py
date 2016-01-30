#!/usr/bin/python
"""
============================================================
Parameter estimation using grid search with cross-validation
============================================================

This examples shows how a classifier is optimized by cross-validation,
which is done using the :class:`sklearn.grid_search.GridSearchCV` object
on a development set that comprises only half of the available labeled data.

The performance of the selected hyper-parameters and trained model is
then measured on a dedicated evaluation set that was not used during
the model selection step.

More details on tools available for model selection can be found in the
sections on :ref:`cross_validation` and :ref:`grid_search`.

"""

from __future__ import print_function

from sklearn import datasets
from sklearn.cross_validation import train_test_split
from sklearn.grid_search import GridSearchCV
from sklearn.metrics import classification_report
from sklearn.svm import SVC
from sklearn.svm import LinearSVC
from scipy import io
from baseline import loaddata,featureselection
import numpy as np
from string import strip
from sklearn.naive_bayes import *
from time import time
from sklearn.neighbors import KNeighborsClassifier as knn
from datagenerator import output
def cross_validation(X_train,y_train,X_test,y_test,categories,feature_names,featureNum,model,tuned_parameters,fold = 10):
	#scores = ['precision', 'recall']
	t = time()
	score = 'f1'
	clf = None
    	print("# Tuning hyper-parameters for %s" % score)
    	print()
    	clf = GridSearchCV(model, tuned_parameters, cv=fold,
                      scoring='%s_weighted' % score)
	print(X_train.shape,y_train.shape,type(y_train))
    	clf.fit(X_train, y_train)

 	print("Best parameters set found on development set:")
   	print()
   	print(clf.best_params_)
    	print()
    	print("Grid scores on development set:")
    	print()
    	for params, mean_score, scores in clf.grid_scores_:
       		print("%0.3f (+/-%0.03f) for %r"
          			% (mean_score, scores.std() * 2, params))
    	print()

   	print("Detailed classification report:")
   	print()
   	print("The model is trained on the full development set.")
   	print("The scores are computed on the full evaluation set.")
    	print()
    	y_true, y_pred = y_test, clf.predict(X_test)
    	print(classification_report(y_true, y_pred))
    	print()
	print(time() - t)
	accuracy = np.mean((y_true==y_pred)*1.0)	
	return (clf,accuracy)
def svmTrain(datafile,featureNum,fold = 10):
	import sys
	train,test = loaddata(datafile)
	row,col = train['counts'].shape
	if col < featureNum:
		featureNum = col 
	X_train = train['counts'][:,0:featureNum]
	y_train = train['labels'][0,:]
	X_test = test['counts'][:,0:featureNum]
	y_test = test['labels'][0,:]
	tuned_parameters = [#{'kernel': ['rbf'], 'gamma': [1e-3, 1e-4],
                     #'C': [1, 10, 100, 1000,10000]},
                    {'C': [1, 10, 100, 1000,10000]}]
	model = LinearSVC(dual = True,tol=1e-3)	 
	categories = train['category']
	feature_names =np.array([k.strip() for k in train['feature_names']])
	data = [X_train,y_train,X_test,y_test,categories,feature_names,featureNum,model,tuned_parameters,fold]
	clf,accuracy = cross_validation(*data)
	'''
	for c in range(len(categories)):
		index = np.argsort(clf.best_estimator_.coef_[c])
		if len(index) >= featureNum:
			topfeatures = index[-1*featureNum:]
		else:
			topfeatures = index
		print('%s:%s'%(categories[c], ' '.join(feature_names[topfeatures])))
	'''
	return accuracy
def NBTrain(datafile,featureNum,fold = 10):
	import sys
	train,test = loaddata(datafile)
	row,col = train['counts'].shape
	if col < featureNum:
		featureNum = col 
	X_train = train['counts'][:,0:featureNum]
	y_train = train['labels'][0,:]
	X_test = test['counts'][:,0:featureNum]
	y_test = test['labels'][0,:]
	tuned_parameters = [{'alpha':[0.01,0.05,1,2,5]}]
	model = MultinomialNB(fit_prior=True)	 
	categories = train['category']
	feature_names =np.array([k.strip() for k in train['feature_names']])
	data = [X_train,y_train,X_test,y_test,categories,feature_names,featureNum,model,tuned_parameters,fold]
	clf,accuracy = cross_validation(*data)
	for c in range(len(categories)):
		index = np.argsort(clf.best_estimator_.coef_[c])
		if len(index) >= featureNum:
			topfeatures = index[-1*featureNum:]
		else:
			topfeatures = index
		print('%s:%s'%(categories[c], ' '.join(feature_names[topfeatures])))
	return accuracy
def knnTrain(datafile,featureNum,fold = 10):
	import sys
	train,test = loaddata(datafile)
	row,col = train['counts'].shape
	if col < featureNum:
		featureNum = col 
	X_train = train['counts'][:,0:featureNum]
	y_train = train['labels'][0,:]
	X_test = test['counts'][:,0:featureNum]
	y_test = test['labels'][0,:]
	tuned_parameters = [{'n_neighbors':[2,3,4,6,10,15,18,20,30,40,50]}]
	model = knn(n_neighbors = 1)	 
	categories = train['category']
	feature_names =np.array([k.strip() for k in train['feature_names']])
	data = [X_train,y_train,X_test,y_test,categories,feature_names,featureNum,model,tuned_parameters,fold]
	clf,accuracy = cross_validation(*data)
	return accuracy
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
	featureNumb = 100
	'''
	directory = './data'
	feature = sys.argv[1] 
	outprefix = os.path.splitext(feature)[0]
	transformer = io.loadmat('./data/retrive_ori_mtx_indx.mat')['indx_cut'][0,:] - 1
	#feature = ourPreprocess(feature,transformer,original,outprefix)
	'''
	#print(feature,outprefix)	
	feature = preprocess(original,featureNumb)
	svmTrain(feature,featureNumb)
	NBTrain(feature,featureNumb)
	knnTrain(feature,featureNumb)
	'''
	print(__doc__)

	# Loading the Digits dataset
	digits = datasets.load_digits()

	# To apply an classifier on this data, we need to flatten the image, to
	# turn the data in a (samples, feature) matrix:
	n_samples = len(digits.images)
	X = digits.images.reshape((n_samples, -1))
	y  = digits.target

	# Split the dataset in two equal parts
	X_train, X_test, y_train, y_test = train_test_split(
    		X, y, test_size=0.5, random_state=0)
	print(X_train.shape,y_train.shape)
	# Set the parameters by cross-validation
	tuned_parameters = [{'kernel': ['rbf','linear'], 'gamma': [1e-3, 1e-4],
                     'C': [1, 10, 100, 1000]},
                    {'kernel': ['linear'], 'C': [1, 10, 100, 1000]}]
	model = SVC()
	data = [X_train,y_train,X_test,y_test,model,tuned_parameters,2]
	cross_validation(*data)
	#cross_validation(X_train,y_train,X_test,y_test,model,tuned_parameters,fold = 2)
	'''
# Note the problem is too easy: the hyperparameter plateau is too flat and the
# output model is the same for precision and recall with ties in quality.
