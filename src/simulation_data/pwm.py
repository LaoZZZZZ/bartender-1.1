#!/usr/bin/python


def countBp(seq_file):
	counts = [[0]*20 for i in range(4)]
	total = 0
	index = {'A':0, 'C':1, 'G':2, 'T':3}
	for line in open(seq_file, 'r'):
		seq, f = line.strip().split(',')
		f = int(f)
		total += f
		for i in range(len(seq)):
			counts[index[seq[i]]][i] += f
	print(total)
	return counts


if __name__ == '__main__':
	seq_file = 'Simulated_barcode_seq.csv'
	counts = countBp(seq_file)
	output = open('Simulated_pwm.csv','w')
	for bp in counts:
		output.write(','.join(map(str,bp)) + '\n')		
	output.close()
			
			
		
