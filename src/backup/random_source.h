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

#ifndef RANDOM_GEN_H_
#define RANDOM_GEN_H_

#include <stdint.h>
#include "assert_helpers.h"
#include "typdefine.h"
//#define MERSENNE_TWISTER

namespace prefixMatching{

#ifndef MERSENNE_TWISTER

/**
 * Simple pseudo-random linear congruential generator, a la Numerical
 * Recipes.
 */
class RandomSource {

public:
    static const UINT32 DEFUALT_A = 1664525;
    static const UINT32 DEFUALT_C = 1013904223;

	RandomSource() :
		a(DEFUALT_A), c(DEFUALT_C), inited_(false) { }
    RandomSource(UINT32 _last) :
		a(DEFUALT_A), c(DEFUALT_C), last(_last), inited_(true) { }
    RandomSource(UINT32 _a, UINT32 _c) :
		a(_a), c(_c), inited_(false) { }

    void init(UINT32 seed = 0) {
		last = seed;
		inited_ = true;
		lastOff = 30;
	}

    UINT32 nextU32() {
		assert(inited_);
        UINT32 ret;
		last = a * last + c;
		ret = last >> 16;
		last = a * last + c;
		ret ^= last;
		lastOff = 0;
		return ret;
	}

	/**
	 * Return a pseudo-random unsigned 32-bit integer sampled uniformly
	 * from [lo, hi].
	 */
    UINT32 nextU32Range(UINT32 lo, UINT32 hi) {
        UINT32 ret = lo;
		if(hi > lo) {
			ret += (nextU32() % (hi-lo+1));
		}
		return ret;
	}

	/**
	 * Get next 2-bit unsigned integer.
	 */
    UINT32 nextU2() {
		assert(inited_);
		if(lastOff > 30) {
			nextU32();
		}
        UINT32 ret = (last >> lastOff) & 3;
		lastOff += 2;
		return ret;
	}

	/**
	 * Get next boolean.
	 */
	bool nextBool() {
		assert(inited_);
		if(lastOff > 31) {
			nextU32();
		}
        UINT32 ret = (last >> lastOff) & 1;
		lastOff++;
		return ret;
	}
	
	/**
	 * Return an unsigned int chosen by picking randomly from among
	 * options weighted by probabilies supplied as the elements of the
	 * 'weights' array of length 'numWeights'.  The weights should add
	 * to 1.
	 */
    UINT32 nextFromProbs(
		const float* weights,
		size_t numWeights)
	{
		float f = nextFloat();
		float tot = 0.0f; // total weight seen so far
        for(UINT32 i = 0; i < numWeights; i++) {
			tot += weights[i];
			if(f < tot) return i;
		}
        return (UINT32)(numWeights-1);
	}

	float nextFloat() {
		assert(inited_);
		return (float)nextU32() / (float)0xffffffff;
	}

    static UINT32 nextU32(UINT32 last,
                            UINT32 a = DEFUALT_A,
                            UINT32 c = DEFUALT_C)
	{
		return (a * last) + c;
	}
	
    UINT32 currentA() const { return a; }
    UINT32 currentC() const { return c; }
    UINT32 currentLast() const { return last; }

private:
    UINT32 a;
    UINT32 c;
    UINT32 last;
    UINT32 lastOff;
	bool inited_;
};

#else

class RandomSource { // Mersenne Twister random number generator

public:

	// default constructor: uses default seed only if this is the first instance
	RandomSource() {
		reset();
	}
	
	// constructor with 32 bit int as seed
    RandomSource(UINT32 s) {
		init(s);
	}
	
	// constructor with array of size 32 bit ints as seed
    RandomSource(const UINT32* array, int size) {
		init(array, size);
	}
	
	void reset() {
		state_[0] = 0;
		p_ = 0;
		inited_ = false;
	}
	
	virtual ~RandomSource() { }
	
	// the two seed functions
    void init(UINT32); // seed with 32 bit integer
    void init(const UINT32*, int size); // seed with array

	/**
	 * Return next 1-bit unsigned integer.
	 */
	bool nextBool() {
		return (nextU32() & 1) == 0;
	}
	
	/**
	 * Get next unsigned 32-bit integer.
	 */
    inline UINT32 nextU32() {
		assert(inited_);
		if(p_ == n) {
			gen_state(); // new state vector needed
		}
		// gen_state() is split off to be non-inline, because it is only called once
		// in every 624 calls and otherwise irand() would become too big to get inlined
        UINT32 x = state_[p_++];
		x ^= (x >> 11);
		x ^= (x << 7) & 0x9D2C5680UL;
		x ^= (x << 15) & 0xEFC60000UL;
		x ^= (x >> 18);
		return x;
	}
	
	/**
	 * Return next float between 0 and 1.
	 */
	float nextFloat() {
		assert(inited_);
		return (float)nextU32() / (float)0xffffffff;
	}
	
protected: // used by derived classes, otherwise not accessible; use the ()-operator

	static const int n = 624, m = 397; // compile time constants

	// the variables below are static (no duplicates can exist)
    UINT32 state_[n]; // state vector array
	int p_; // position in state array
	
	bool inited_; // true if init function has been called
	
	// private functions used to generate the pseudo random numbers
    UINT32 twiddle(UINT32 u, UINT32 v) {
		return (((u & 0x80000000UL) | (v & 0x7FFFFFFFUL)) >> 1) ^ ((v & 1UL) ? 0x9908B0DFUL : 0x0UL);
	}
	
	void gen_state(); // generate new state
	
};

#endif
} //namespace algn_ppbwt
#endif /*RANDOM_GEN_H_*/
