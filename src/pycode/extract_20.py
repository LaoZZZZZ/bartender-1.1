#!/usr/bin/python
from bitwise import BitWiseTransformer
def extract_barcode(f,b_len):
	table = {}
	count = 0
	for line in open(f, 'r'):
		count = count + 1
		if count == 1:
			continue
		parts = line.strip().split(',')
		try:
			if int(parts[1]) == b_len:
				table[parts[0]] =parts[2]
		except Exception as err:
			print(parts[1])
			print(parts)
		
	return table

if __name__ == '__main__':
	f = "../data/single_end_data/single_end_data_count.csv"
	table = extract_barcode(f, 20)
	output = open('../data/single_end_data/single_end_data_20_count.csv','w')
	trans = BitWiseTransformer()
	for k,v in table.items():
		output.write(k + ',' + trans.bitwise(int(k),20) + ',' + v + '\n')
	output.close()
	
	
	
