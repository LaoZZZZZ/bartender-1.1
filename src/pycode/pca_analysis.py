#!/usr/bin/python

from __future__ import print_function

from sklearn.decomposition import TruncatedSVD
from sklearn.feature_extraction.text import TfidfVectorizer

from sklearn.pipeline import make_pipeline
from sklearn.preprocessing import Normalizer
from sklearn.preprocessing import normalize
from sklearn.cluster import KMeans, MiniBatchKMeans


import sys
import os
import numpy as np
import scipy
from scipy import io
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from time import time

from baseline import loaddata
from grid_search_cv import svmTrain,NBTrain,knnTrain
from scipy.sparse import linalg

from outPutMedia import outPutMedia

def screePlot(filename,S):
	o = outPutMedia(filename)
	eigvals = S**2/np.cumsum(S)[-1]
	eigvals2 = S**2/np.sum(S)
	assert (eigvals == eigvals2).all()
	x = np.arange(len(S)) + 1
	print(len(x))
	plt.plot(x,eigvals,'ro-',linewidth = 2)
	plt.title('Scree Plot')
	plt.xlabel('Principle Component')
	plt.ylabel('Eigenvalue')
	leg = plt.legend(['Eigenvalues from Bagofword'],loc='best',borderpad=0.3,shadow=False,markerscale = 0.4)
	leg.get_frame().set_alpha(0.4)
	o.write(None)
	o.close()	
def pca_analysis(X_train,X_test,n_components):
	t0 = time()
	normalized_train = normalize(X_train,norm='l1',axis = 0)
	normalized_test = normalize(X_test,norm='l1',axis = 0)
	
	svd = TruncatedSVD(n_components)
	#lsa = make_pipeline(svd,Normalizer(copy=False))
	#x_train = lsa.fit_transform(normalized_train)
	x_train = svd.fit_transform(normalized_train)
	x_test = svd.transform(normalized_test)
	#x_test = lsa.transform(normalized_test)
	explained_variance = svd.explained_variance_ratio_.sum()
	#print(svd.explained_variance_ratio_)
	#print('Explained variance of the svd step:{}%'.format(int(explained_variance*100)))
	print('done in %fs'%(time() - t0))
	return (x_train,x_test,svd.explained_variance_ratio_)
def pca_analysis_dense(X_train,X_trest,n_components):
	u,s,v = linalg.svds(X_train,n_components)
	print(s.shape)	
	screePlot('original.pdf',s[::-1])
	normalized = normalize(X_train,norm='l1',axis = 0)
	u,s,v = linalg.svds(normalized,n_components)
	print(s.shape)	
	screePlot('normalized.pdf',s[::-1])
def pca_pipeline(prefix,n_components,model):
	train,test = loaddata(prefix)
	X_train = train['counts']
	X_test = test['counts']
	pca_analysis(X_train,X_test,n_components)	
	X_train,X_test,pcs = pca_analysis(X_train,X_test,n_components)
	train['counts'] = X_train

	test['counts'] = X_test

	# setup the categories

	name = 'pc_'
	categories = []
	for i in range(n_components):
		categories.append(name + str(i))
	train['category'] = categories
	test['category'] = categories		
	outprefix = prefix + '_pca'
	io.savemat(outprefix + '_train.mat',train)
	io.savemat(outprefix + '_test.mat',test)
	#svmTrain(outprefix,n_components)	
	accuracy = model(outprefix,n_components)	
	return accuracy
def plotVarianceExplained(prefix,components,outfile):
	o = outPutMedia(outfile + '.pdf')
	x = np.arange(components) + 1
	variance = np.arange(components)
	for c in range(components):
		variance[c] = drive(prefix,x[c])
	result = '\n'.join([str(k) + ',' + str(v) for k,v in zip(x,variance)])
	open(outfile + '.csv','w').write(result)
	plt.plot(x,variance,'ro-',linewidth = 2)
	plt.title('principle number vs variance explained')
	plt.xlabel('Number of principle components')
	plt.ylabel('Explained variance')	
	o.write(None)
	o.close()	
def pca_prediction(prefix,components,outfile,model):
	o = outPutMedia(outfile + '.pdf')
	x = components
	variance = []
	for c in components:
		variance.append(pca_pipeline(prefix,c,model))
	result = '\n'.join([str(k) + ',' + str(v) for k,v in zip(x,variance)])
	open(outfile + '.csv','w').write(result)
	plt.plot(x,variance,'ro-',linewidth = 2)
	plt.title('principle number vs variance explained')
	plt.xlabel('Number of principle components')
	plt.ylabel('Explained variance')	
	o.write(None)
	o.close()	
if __name__ == '__main__':
	prefix = '../features/bagofword'
	#drive(prefix,2)
	method = svmTrain 
	components = [1,5,10,15,20,30,40,80,100,150,200,250,300]
	#components = [1,5]
	pca_prediction(prefix,components,'pca_prediction_svm',method)
	

