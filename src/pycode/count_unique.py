#!/usr/bin/python
import os
import sys

def count(f):
	res = {}
	for line in open(f, 'r'):
		k,v = line.rstrip().split()
		if k in res:
			res[k] += v
		else:
			res[k] = v
	return res


if __name__ == "__main__":
	f = "Simulated_barcode_freq.txt"
	print(len(count(f)))
