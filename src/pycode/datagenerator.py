#!/usr/bin/python
from scipy import io
from scipy.sparse import csr_matrix
from sklearn.datasets import fetch_20newsgroups
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfVectorizer
import numpy as np
import sys
def download():
	#train = fetch_20newsgroups(subset='train',shuffle=True,random_state=42,remove=('headers','footers','quotes'))
	#test = fetch_20newsgroups(subset='test',shuffle=True,random_state=42,remove=('headers','footers','quotes'))
	train = fetch_20newsgroups(subset='train',shuffle=True,random_state=42)
	test = fetch_20newsgroups(subset='test',shuffle=True,random_state=42)
	return (train,test)
# extract bag of words feature and output it
def bagOfWords(twenty,outprefix):
	count_vect = CountVectorizer(stop_words='english',max_df = 0.5,max_features= 10000)
	word_counts = count_vect.fit_transform(twenty.data)
	featurenames = np.asarray(count_vect.get_feature_names())
	output(word_counts/1.0,featurenames,twenty,outprefix)
	return count_vect
def output(word_counts,feature_names,twenty,outprefix):
	data = {'counts':word_counts.tocsr(),'feature_names':feature_names,'labels':twenty.target,'category':twenty.target_names}
	io.savemat(outprefix+'.mat',data)	
# extract term frequency feature from the data
def termFreq(twenty,outprefix):
	#count_vect = CountVectorizer()
	#word_counts = count_vect.fit_transform(twenty.data)
	tf_transformer = TfidfVectorizer(use_idf=False,max_df = 0.5,max_features= 10000,stop_words='english')
	tf_counts = tf_transformer.fit_transform(twenty.data)
	featurenames = np.asarray(tf_transformer.get_feature_names())
	output(tf_counts,featurenames,twenty,outprefix)
	return tf_transformer
# extract term frequency feature invers the document feature from the data
def termFreqInverseDoc(twenty,outprefix):
	#count_vect = CountVectorizer()
	#word_counts = count_vect.fit_transform(twenty.data)
	tfidf_transformer = TfidfVectorizer(stop_words='english',max_df=0.5,max_features=10000)
	in_tfidf = tfidf_transformer.fit_transform(twenty.data)
	featurenames = np.asarray(tfidf_transformer.get_feature_names())
	output(in_tfidf,featurenames,twenty,outprefix)
	return tfidf_transformer
if __name__ == '__main__':
	import os
	prefix = './'
	train,test = download()
	transformer = bagOfWords(train,os.path.join(prefix,'bagofword_train'))
	featurenames = np.asarray(transformer.get_feature_names())
	feature = transformer.transform(test.data)
	output(feature/1.0,featurenames,test,os.path.join(prefix,'bagofword_test'))
	'''
	#bagOfWords(test,'bagofword_test')
	transformer = termFreq(train,os.path.join(prefix,'termfreq_train'))
	featurenames = np.asarray(transformer.get_feature_names())
	feature = transformer.transform(test.data)
	output(feature,featurenames,test,os.path.join(prefix,'termfreq_test'))
	#termFreq(test,'termfreq_test')

	transformer = termFreqInverseDoc(train,os.path.join(prefix,'termfreqinverse_train'))
	featurenames = np.asarray(transformer.get_feature_names())
	feature = transformer.transform(test.data)
	output(feature,featurenames,test,os.path.join(prefix,'termfreqinverse_test'))
	#termFreqInverseDoc(test,'termfreqinverse_test_in')
	'''
