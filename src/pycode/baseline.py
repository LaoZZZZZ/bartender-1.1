#!/usr/bin/python

import logging
import numpy as np
#from optparse import OptionParser
import sys
from time import time
import matplotlib.pyplot as plt

from sklearn.datasets import fetch_20newsgroups
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.feature_extraction.text import HashingVectorizer
from sklearn.feature_selection import SelectKBest, chi2
from sklearn.linear_model import RidgeClassifier
from sklearn.pipeline import Pipeline
from sklearn.svm import LinearSVC
from sklearn.linear_model import SGDClassifier
from sklearn.linear_model import Perceptron
from sklearn.linear_model import PassiveAggressiveClassifier
from sklearn.naive_bayes import BernoulliNB, MultinomialNB
from sklearn.neighbors import KNeighborsClassifier
from sklearn.neighbors import NearestCentroid
from sklearn.ensemble import RandomForestClassifier
from sklearn.utils.extmath import density
from sklearn import metrics
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
from benchMark import benchmark
# download the data from the web
def dataload():
	newsgroups_train = fetch_20newsgroups(subset='train',shuffle=True, random_state=42)
	newsgroups_test = fetch_20newsgroups(subset='test',shuffle=True, random_state=42)
	print('data loaded')
	return (newsgroups_train,newsgroups_test)

# select the corresponding features
# could be bag of word
# tfidf
# tfidf_in


def loaddata(featurefile):
	from scipy import io
	train = io.loadmat(featurefile+'_train.mat')
	test = io.loadmat(featurefile+'_test.mat')
	return (train,test)


# using the chi-square to select features
def featureselection(data,featurenumber):
	ch2 = SelectKBest(chi2,k=featurenumber)
	x_train = ch2.fit_transform(data[0]['counts'],np.transpose(data[0]['labels']))
	x_test = ch2.transform(data[1]['counts'])
	feature_names = [data[0]['feature_names'][i] for i in ch2.get_support(indices=True)]
	return (x_train,x_test,feature_names)


if __name__ =='__main__':
	featurefile = 'bagofword'
	train,test = loaddata(featurefile)
	#xtrain,xtest = featureselection((train,test),1000)
	xtrain,xtest = train['counts'],test['counts'] 
	print(xtrain.shape,xtest.shape)
	ytrain=np.transpose(train['labels'])
	ytest=np.transpose(test['labels'])
	data = [xtrain,ytrain,xtest,ytest]		
	results = []
	for clf, name in (
        	(RidgeClassifier(tol=1e-2, solver="lsqr"), "Ridge Classifier"),
        	(Perceptron(n_iter=50), "Perceptron"),
        	(PassiveAggressiveClassifier(n_iter=50), "Passive-Aggressive"),
        	(KNeighborsClassifier(n_neighbors=10), "kNN"),
        	(RandomForestClassifier(n_estimators=100), "Random forest")):
    		print('=' * 80)
    		print(name)
    		results.append(benchmark(clf,*data))	
	# svm
	for penalty in ["l2", "l1"]:
    		print('=' * 80)
    		print("%s penalty" % penalty.upper())
    		# Train Liblinear model
    		results.append(benchmark(LinearSVC(loss='l2', penalty=penalty,
                                            dual=False, tol=1e-3),*data))

    		# Train SGD model
    		results.append(benchmark(SGDClassifier(alpha=.0001, n_iter=50,
                                           penalty=penalty),*data))


	# train SGD SGD with Elastic Net penalty
	print('=' * 80)
	print("Elastic-Net penalty")
	results.append(benchmark(SGDClassifier(alpha=.0001, n_iter=50,
                                       penalty="elasticnet"),*data))

	# Train NearestCentroid without threshold
	print('=' * 80)
	print("NearestCentroid (aka Rocchio classifier)")
	results.append(benchmark(NearestCentroid(),*data))

	# Train sparse Naive Bayes classifiers
	print('=' * 80)
	print("Naive Bayes")
	results.append(benchmark(MultinomialNB(alpha=.01),*data))
	results.append(benchmark(BernoulliNB(alpha=.01),*data))

	print('=' * 80)
	print("LinearSVC with L1-based feature selection")
	# The smaller C, the stronger the regularization.
	# The more regularization, the more sparsity.
	results.append(benchmark(Pipeline([
  		('feature_selection', LinearSVC(penalty="l1", dual=False, tol=1e-3)),
  		('classification', LinearSVC())
		]),*data))

'''

categories = data.target_names
featuresNumb = 100
X_train = in_tfidf_train
Y_train = newsgroups_train.target

X_test = in_tfidf_test
Y_test = newsgroups_test.target 

ch2 = SelectKBest(chi2, k=featuresNumb)
X_train = ch2.fit_transform(X_train, Y_train)
X_test = ch2.transform(X_test)

x_'''


