#!/usr/bin/python
import numpy as np
import sys
def loadBarcode(filename):
	result = {}
	for line in open(filename, 'r'):
		barcode, lineNum = line.strip().split(',')
		if barcode in result:
			result[barcode] += 1
		else:
			result[barcode] = 1
	return result

# random generate umi for each uniq barcode
def generateUmi(barcodeCount, dictionary, umiLength, mutableProb):
	result = []
	for k,f in barcodeCount.iteritems():
		umi = np.random.choice(dictionary, umiLength)
		for i in range(f):
			mutatedUmi = np.copy(umi)
			for i in range(umiLength):
				if np.random.binomial(1, mutableProb) == 1:
					mutatedUmi[i] = np.random.choice(dictionary, 1)[0]
			result.append(k + ',' + ''.join(mutatedUmi))
	return result

if __name__ == '__main__':
	filename = "2M_extracted_barcode.txt"
    	dictionary = ['A', 'C', 'G', 'T', 'N']
	barcodeCount = loadBarcode(filename)
	
	np.random.seed(23)	
	barcodeWithUmi = generateUmi(barcodeCount, dictionary, 10, 0.1)
	outputfile = "2M_extracted_barcode_umi.txt"
	handler = open(outputfile, 'w').write('\n'.join(barcodeWithUmi))
