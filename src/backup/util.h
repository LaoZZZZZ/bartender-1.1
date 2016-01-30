/*
 * Copyright 2011, Ben Langmead <langmea@cs.jhu.edu>
 *
 * This file is part of Bowtie 2.
 *
 * Bowtie 2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bowtie 2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Bowtie 2.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdlib.h>
#include <limits>
#include <string>
#include <stdexcept>
#include "suffix_commons.h"
#include "kmer_freq.h"
#include "binaryinfilebuf.h"
#include "binaryoutfilebuf.h"
#include "barcodecounting.h"
#include "cluster.h"
using std::string;
using std::runtime_error;


namespace prefixMatching {

/**
 * C++ version char* style "itoa":
 */
/*
template<typename T>
char* itoa10(const T& value, char* result) {
	// Check that base is valid
	char* out = result;
	T quotient = value;
	if(std::numeric_limits<T>::is_signed) {
		if(quotient <= 0) quotient = -quotient;
	}
	// Now write each digit from most to least significant
	do {
		*out = "0123456789"[quotient % 10];
		++out;
		quotient /= 10;
	} while (quotient > 0);
	// Only apply negative sign for base 10
	if(std::numeric_limits<T>::is_signed) {
		// Avoid compiler warning in cases where T is unsigned
		if (value <= 0 && value != 0) *out++ = '-';
	}
	reverse( result, out );
	*out = 0; // terminator
	return out;
}*/
string reverseComplement(const string& seq);
template<typename T>
int     hammingDist(const T& seq1,const T& seq2){
    int dist = 0;
    if(seq1.length() != seq2.length()){
        throw runtime_error(" Can not calculate hamming distance between unequal length strings!\n");
    }else{
        size_t l = seq1.length();
        for(size_t i = 0; i < l ; i++){
            if(seq1[i] != seq2[i])
                dist++;
        }
    }
    return dist;
}

size_t kmerDistance(const kmer&,const kmer&,size_t);
/**
  * Calculate the
*/
double similarityScore(const kmer&,const list<kmers_freq>&,size_t);
void outputBarcode(std::string,const kmers_sparseHash<kmer,freq>&);
void loadBarcode(std::string,kmers_sparseHash<kmer,freq>&);
void outputCluster(std::fstream& handler,std::shared_ptr<cluster>& c);
std::list<std::string> strSplit(const std::string&);
void strSplit(const std::string&,std::list<std::string>&);
}
#endif /*ndef UTIL_H_*/
