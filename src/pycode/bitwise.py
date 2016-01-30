#!/usr/bin/python

class BitWiseTransformer:
	def __init__(self):
		self.__dict = {0:'A', 1:'C', 2:'G', 3:'T'}
	def bitwise(self,seq, k):
		res = ''
		for i in range(k):
			res += self.__dict[(seq & 3)]
			seq = seq >> 2
		return res[::-1]


if __name__ == '__main__':
	print(BitWiseTransformer().bitwise(794264584047,20))
	
