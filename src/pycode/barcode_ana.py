#!/usr/bin/python

import numpy as np
import scipy as scp

from matplotlib import pylab as plt
import pylab as P
from operator import itemgetter

def loadData(barcodefile,deli = None):
	barcodes={}
	for line in open(barcodefile,'r'):
		if deli == None:
			rec = line.strip().split()
		else:
			rec = line.strip().split(deli)
		if not rec[0] in barcodes:	
			barcodes[rec[0]] = int(rec[1])
		else:
			barcodes[rec[0]] += int(rec[1])
	return barcodes


def checkConsistency(truth,predicted):
	total = 0
	for k,v in truth.items():
		total += v
	print(total)	
	total = 0
	for k,v in predicted.items():
		total += v
	print(total)	
	centers = dict([(k,v) for k,v in predicted.items() if v > 3])	
	junk = dict([(k,v) for k,v in predicted.items() if v <=3])	
	trueBarcode = [(k,v) for k,v in truth.items() if v > 3]
	TP = 0.0
	FN = 0.0
	frequencies = []
	difffreq = []
	for k,v in trueBarcode:
		if k in centers:
			TP += 1
			difffreq.append((v,centers[k]))
		elif k in junk:
			FN += 1	
		else:
			frequencies.append(v)
		
	
	print('True positive is ' + str(TP/len(trueBarcode)))
	print('False negative is ' + str(FN/len(trueBarcode)))
	print('barcode famility size: predicted: ' + str(len(centers)) + ', truth:' + str(len(trueBarcode)))	
	print('Percentage of barcode that cluster size is larger than 3: ' + str(len(centers)*1.0/len(predicted)))
	print('False negative barcode frequency percentile: ' +  ','.join(map(str,np.percentile(frequencies,[0,10,25,50,75,90,99,100]))))		
	return difffreq
if __name__ == '__main__':
	
	truth = loadData('../data/True_barcode_freq.txt')
	cluster = loadData('../data/clusters.csv',',')
	#print(sum(truth.values()),sum(cluster.values()))
	freq = checkConsistency(truth,cluster)
	'''
	freq = sorted(freq,key=itemgetter(0))	
	x,y = zip(*freq)
	P.figure()
	P.plot(x,y)
	#x = [ v for v in data.values() if v > 5]
	#x = [ v for v in cluster.values() if v > 3]
	#n,bins,patches = P.hist(x,50,normed=1)
	P.show()
	'''
