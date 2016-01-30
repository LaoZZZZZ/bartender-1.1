#!/usr/bin/python
import os
from bitwise import BitWiseTransformer

def transform_2_seq(count_table_file, skip = 0):
	table = {}
	transformer = BitWiseTransformer()
	count = 0
	for line in open(count_table_file,'r'):
		count = count + 1
		if count <= skip:
			continue 
		if ',' in line:
			l = line.strip().split(',')
		else:
			l = line.strip().split('\t')
		if len(l) == 3:
			kmer = transformer.bitwise(int(l[0]),int(l[1]))
		else:
			kmer = transformer.bitwise(int(l[0]), 20)
		if kmer in table:
			print(','.join(table[kmer]))
			print(line)
			raise RuntimeError("duplicate found")
		else:
			table[kmer] = l
	return table
def parseClusterResult(result_file):
	table = {}
	transformer = BitWiseTransformer()
	count = 0  
	header = ''
	for line in open(result_file, 'r'):
		count = count + 1	
		if count == 1:
			print(line)
			header = line.strip()
			continue
		parts = line.strip().split(',')
		kmer = transformer.bitwise(int(parts[2]), 20)
		if kmer in table:
			print(','.join(table[kmer]))
			print(line)
			raise RuntimeError("duplicate found!\n")
		else:
			table[kmer] = parts
	return (header,table)			

if __name__ == '__main__':
	'''
	os.chdir("/home/laozzzzz/Documents/barcode_single_end/BarcodeSimulation/")
	
	trans = BitWiseTransformer()
	time_points = range(1,162,8)
	truth_files = ['result_' + str(t) + '_cluster.csv' for t in time_points]
	print(time_points, truth_files)	
	truth_files = ['combine_result_cluster.csv']
	for f in truth_files:
		header, table = parseClusterResult(f)
		print(len(table))
		prefix = f.split('.')[0]
		handler = open(prefix + '_seq.csv', 'w')
		handler.write(header + ',center_sequence\n')
		for k,v in table.items():
			handler.write(','.join(v) + ',' + k + '\n')
		handler.close()
	'''
	truth = '/Users/lukez/Documents/barcode_single_end/simulation_data/Truth_time_point_'
	#simulated = '/Users/lukez/Documents/barcode_single_end/simulation_data/Simulated_barcode_freq.txt'
	#simulated = '/Users/lukez/Documents/barcode_single_end/simulation_data/2M_time_point_0.csv'
	#cluster_result = '/home/laozzzzz/Documents/barcode_single_end/simulation_data/cluster_cluster.csv'
	for i in range(21):
		input = truth + str(1 + i * 8) + '.csv'
		table = transform_2_seq(input, 1)
		output = truth + str(1 + i * 8) + '_seq.csv'
		print(output)
		
		handle = open(output,'w')
		for k, v in table.items():
			handle.write(k + ',' + v[-1] + '\n')	
		handle.close()
	'''	
	table = transform_2_seq(truth,0)
	print(len(table)) 	
	output = '/Users/lukez/Documents/barcode_single_end/simulation_data/True_barcode_seq.csv'
	#table = parseClusterResult(cluster_result)
	handle = open(output,'w')
	for k, v in table.items():
		handle.write(k + ',' + v[-1] + '\n')	
	handle.close()
	'''
