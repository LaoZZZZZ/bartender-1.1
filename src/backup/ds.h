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

#ifndef DS_H_
#define DS_H_

#include <algorithm>
#include <stdexcept>
#include <utility>
#include <stdint.h>
#include <string.h>
#include <limits>

#include "assert_helpers.h"
#include "random_source.h"
namespace prefixMatching {

typedef uint32_t UINT32;

/**
 * An EList<T> is an expandable list with these features:
 *
 *  - Payload type is a template parameter T.
 *  - Initial size can be specified at construction time, otherwise
 *    default of 128 is used.
 *  - When allocated initially or when expanding, the new[] operator is
 *    used, which in turn calls the default constructor for T.
 *  - All copies (e.g. assignment of a const T& to an EList<T> element,
 *    or during expansion) use operator=.
 *  - When the EList<T> is resized to a smaller size (or cleared, which
 *    is like resizing to size 0), the underlying containing is not
 *    reshaped.  Thus, ELists<T>s never release memory before
 *    destruction.
 *
 * And these requirements:
 *
 *  - Payload type T must have a default constructor.
 *
 * For efficiency reasons, ELists should not be declared on the stack
 * in often-called worker functions.  Best practice is to declare
 * ELists atcontain many a relatively stable layer of the stack (such that it
 * rarely bounces in and out of scope) and let the worker function use
 * it and *expand* it only as needed.  The effect is that only
 * relatively few allocations and copies will be incurred, and they'll
 * occur toward the beginning of the computation before stabilizing at
 * a "high water mark" for the remainder of the computation.
 *
 * A word about multidimensional lists.  One way to achieve a
 * multidimensional lists is to nest ELists.  This works, but it often
 * involves a lot more calls to the default constructor and to
 * operator=, especially when the outermost EList needs expanding, than
 * some of the alternatives.  One alternative is use a most specialized
 * container that still uses ELists but knows to use xfer instead of
 * operator= when T=EList.
 *
 * The 'cat_' fiends encodes a category.  This makes it possible to
 * distinguish between object subgroups in the global memory tally.
 *
 * Memory allocation is lazy.  Allocation is only triggered when the
 * user calls push_back, expand, resize, or another function that
 * increases the size of the list.  This saves memory and also makes it
 * easier to deal with nested ELists, since the default constructor
 * doesn't set anything in stone.
 */
template <typename T, int S = 20>
class EList {

public:

	/**
	 * Allocate initial default of S elements.
	 */
	explicit EList() :
		cat_(0), allocCat_(-1), list_(NULL), sz_(S), cur_(0) { }

	/**
	 * Allocate initial default of S elements.
	 */
	explicit EList(int cat) :
		cat_(cat), allocCat_(-1), list_(NULL), sz_(S), cur_(0)
	{
		assert_geq(cat, 0);
	}

	/**
	 * Initially allocate given number of elements; should be > 0.
	 */
    explicit EList(size_t isz, int cat = 0) :
        cat_(cat), allocCat_(-1), list_(NULL), sz_(isz), cur_(0)
	{
		assert_geq(cat, 0);
	}

	/**
	 * Copy from another EList using operator=.
	 */
	EList(const EList<T, S>& o) :
		cat_(0), allocCat_(-1), list_(NULL), sz_(0), cur_(0)
	{
		*this = o;
	}

	/**
	 * Copy from another EList using operator=.
	 */
	explicit EList(const EList<T, S>& o, int cat) :
		cat_(cat), allocCat_(-1), list_(NULL), sz_(0), cur_(0)
	{
		*this = o;
		assert_geq(cat, 0);
	}

	/**
	 * Destructor.
	 */
	~EList() { free(); }

	/**
	 * Make this object into a copy of o by allocat
	 */
	EList<T, S>& operator=(const EList<T, S>& o) {
		assert_eq(cat_, o.cat());
		if(o.cur_ == 0) {
			// Nothing to copy
			cur_ = 0;
			return *this;
		}
		if(list_ == NULL) {
			// cat_ should already be set
			lazyInit();
		}
		if(sz_ < o.cur_) expandNoCopy(o.cur_ + 1);
		assert_geq(sz_, o.cur_);
		cur_ = o.cur_;
		for(size_t i = 0; i < cur_; i++) {
			list_[i] = o.list_[i];
		}
		return *this;
	}

	/**
	 * Transfer the guts of another EList into this one without using
	 * operator=, etc.  We have to set EList o's list_ field to NULL to
	 * avoid o's destructor from deleting list_ out from under us.
	 */
	void xfer(EList<T, S>& o) {
		// What does it mean to transfer to a different-category list?
		assert_eq(cat_, o.cat());
		// Can only transfer into an empty object
		free();
		allocCat_ = cat_;
		list_ = o.list_;
		sz_ = o.sz_;
		cur_ = o.cur_;
		o.list_ = NULL;
		o.sz_ = o.cur_ = 0;
		o.allocCat_ = -1;
	}

	/**
	 * Return number of elements.
	 */
	inline size_t size() const { return cur_; }

	/**
	 * Return number of elements allocated.
	 */
	inline size_t capacity() const { return sz_; }

	/**
	 * Return the total size in bytes occupied by this list.
	 */
	size_t totalSizeBytes() const {
		return 	2 * sizeof(int) +
		        2 * sizeof(size_t) +
				cur_ * sizeof(T);
	}

	/**
	 * Return the total capacity in bytes occupied by this list.
	 */
	size_t totalCapacityBytes() const {
		return 	2 * sizeof(int) +
		        2 * sizeof(size_t) +
				sz_ * sizeof(T);
	}

	/**
	 * Ensure that there is sufficient capacity to expand to include
	 * 'thresh' more elements without having to expand.
	 */
	inline void ensure(size_t thresh) {
		if(list_ == NULL) lazyInit();
		expandCopy(cur_ + thresh);
	}

	/**
	 * Ensure that there is sufficient capacity to include 'newsz' elements.
	 * If there isn't enough capacity right now, expand capacity to exactly
	 * equal 'newsz'.
	 */
	inline void reserveExact(size_t newsz) {
		if(list_ == NULL) lazyInitExact(newsz);
		expandCopyExact(newsz);
	}

	/**
	 * Return true iff there are no elements.
	 */
	inline bool empty() const { return cur_ == 0; }

	/**
	 * Return true iff list hasn't been initialized yet.
	 */
	inline bool null() const { return list_ == NULL; }

	/**
	 * Add an element to the back and immediately initialize it via
	 * operator=.
	 */
	void push_back(const T& el) {
		if(list_ == NULL) lazyInit();
		if(cur_ == sz_) expandCopy(sz_+1);
		list_[cur_++] = el;
	}

	/**
	 * Add an element to the back.  No intialization is done.
	 */
	void expand() {
		if(list_ == NULL) lazyInit();
		if(cur_ == sz_) expandCopy(sz_+1);
		cur_++;
	}

	/**
	 * Add an element to the back.  No intialization is done.
	 */
	void fill(size_t begin, size_t end, const T& v) {
		assert_leq(begin, end);
		assert_leq(end, cur_);
		for(size_t i = begin; i < end; i++) {
			list_[i] = v;
		}
	}

	/**
	 * Add an element to the back.  No intialization is done.
	 */
	void fill(const T& v) {
		for(size_t i = 0; i < cur_; i++) {
			list_[i] = v;
		}
	}

	/**
	 * Set all bits in specified range of elements in list array to 0.
	 */
	void fillZero(size_t begin, size_t end) {
		assert_leq(begin, end);
		memset(&list_[begin], 0, sizeof(T) * (end-begin));
	}

	/**
	 * Set all bits in the list array to 0.
	 */
	void fillZero() {
		memset(list_, 0, sizeof(T) * cur_);
	}

	/**
	 * If size is less than requested size, resize up to at least sz
	 * and set cur_ to requested sz.
	 */
	void resizeNoCopy(size_t sz) {
		if(sz > 0 && list_ == NULL) lazyInit();
		if(sz <= cur_) {
			cur_ = sz;
			return;
		}
		if(sz_ < sz) expandNoCopy(sz);
		cur_ = sz;
	}

	/**
	 * If size is less than requested size, resize up to at least sz
	 * and set cur_ to requested sz.
	 */
	void resize(size_t sz) {
		if(sz > 0 && list_ == NULL) lazyInit();
		if(sz <= cur_) {
			cur_ = sz;
			return;
		}
		if(sz_ < sz) {
			expandCopy(sz);
		}
		cur_ = sz;
	}

	/**
	 * If size is less than requested size, resize up to exactly sz and set
	 * cur_ to requested sz.
	 */
	void resizeExact(size_t sz) {
		if(sz > 0 && list_ == NULL) lazyInitExact(sz);
		if(sz <= cur_) {
			cur_ = sz;
			return;
		}
		if(sz_ < sz) expandCopyExact(sz);
		cur_ = sz;
	}

	/**
	 * Erase element at offset idx.
	 */
	void erase(size_t idx) {
		assert_lt(idx, cur_);
		for(size_t i = idx; i < cur_-1; i++) {
			list_[i] = list_[i+1];
		}
		cur_--;
	}

	/**
	 * Erase range of elements starting at offset idx and going for len.
	 */
	void erase(size_t idx, size_t len) {
		assert_geq(len, 0);
		if(len == 0) {
			return;
		}
		assert_lt(idx, cur_);
		for(size_t i = idx; i < cur_-len; i++) {
			list_[i] = list_[i+len];
		}
		cur_ -= len;
	}

	/**
	 * Insert value 'el' at offset 'idx'
	 */
	void insert(const T& el, size_t idx) {
		if(list_ == NULL) lazyInit();
		assert_leq(idx, cur_);
		if(cur_ == sz_) expandCopy(sz_+1);
		for(size_t i = cur_; i > idx; i--) {
			list_[i] = list_[i-1];
		}
		list_[idx] = el;
		cur_++;
	}

	/**
	 * Insert contents of list 'l' at offset 'idx'
	 */
	void insert(const EList<T>& l, size_t idx) {
		if(list_ == NULL) lazyInit();
        assert_lt(idx, cur_);
		if(l.cur_ == 0) return;
		if(cur_ + l.cur_ > sz_) expandCopy(cur_ + l.cur_);
		for(size_t i = cur_ + l.cur_ - 1; i > idx + (l.cur_ - 1); i--) {
			list_[i] = list_[i - l.cur_];
		}
		for(size_t i = 0; i < l.cur_; i++) {
			list_[i+idx] = l.list_[i];
		}
		cur_ += l.cur_;
	}

	/**
	 * Remove an element from the top of the stack.
	 */
	void pop_back() {
		assert_gt(cur_, 0);
		cur_--;
	}

	/**
	 * Make the stack empty.
	 */
	void clear() {
		cur_ = 0; // re-use stack memory
		// Don't clear heap; re-use it
	}

	/**
	 * Get the element on the top of the stack.
	 */
	inline T& back() {
		assert_gt(cur_, 0);
		return list_[cur_-1];
	}

	/**
	 * Reverse list elements.
	 */
	void reverse() {
		if(cur_ > 1) {
			size_t n = cur_ >> 1;
			for(size_t i = 0; i < n; i++) {
				T tmp = list_[i];
				list_[i] = list_[cur_ - i - 1];
				list_[cur_ - i - 1] = tmp;
			}
		}
	}

	/**
	 * Get the element on the top of the stack, const version.
	 */
	inline const T& back() const {
		assert_gt(cur_, 0);
		return list_[cur_-1];
	}

	/**
	 * Get the frontmost element (bottom of stack).
	 */
	inline T& front() {
		assert_gt(cur_, 0);
		return list_[0];
	}

	/**
	 * Get the element on the bottom of the stack, const version.
	 */
	inline const T& front() const { return front(); }

	/**
	 * Return true iff this list and list o contain the same elements in the
	 * same order according to type T's operator==.
	 */
	bool operator==(const EList<T, S>& o) const {
		if(size() != o.size()) {
			return false;
		}
		for(size_t i = 0; i < size(); i++) {
			if(!(get(i) == o.get(i))) {
				return false;
			}
		}
		return true;
	}

	/**
	 * Return true iff this list contains all of the elements in o according to
	 * type T's operator==.
	 */
	bool isSuperset(const EList<T, S>& o) const {
		if(o.size() > size()) {
			// This can't be a superset if the other set contains more elts
			return false;
		}
		// For each element in o
		for(size_t i = 0; i < o.size(); i++) {
			bool inthis = false;
			// Check if it's in this
			for(size_t j = 0; j < size(); j++) {
				if(o[i] == (*this)[j]) {
					inthis = true;
					break;
				}
			}
			if(!inthis) {
				return false;
			}
		}
		return true;
	}

	/**
	 * Return a reference to the ith element.
	 */
	inline T& operator[](size_t i) {
		assert_lt(i, cur_);
		return list_[i];
	}

	/**
	 * Return a reference to the ith element.
	 */
	inline const T& operator[](size_t i) const {
		assert_lt(i, cur_);
		return list_[i];
	}

	/**
	 * Return a reference to the ith element.
	 */
	inline T& get(size_t i) {
		return operator[](i);
	}

	/**
	 * Return a reference to the ith element.
	 */
	inline const T& get(size_t i) const {
		return operator[](i);
	}

	/**
	 * Return a reference to the ith element.  This version is not
	 * inlined, which guarantees we can use it from the debugger.
	 */
	T& getSlow(size_t i) {
		return operator[](i);
	}

	/**
	 * Return a reference to the ith element.  This version is not
	 * inlined, which guarantees we can use it from the debugger.
	 */
	const T& getSlow(size_t i) const {
		return operator[](i);
	}

	/**
	 * Sort some of the contents.
	 */
	void sortPortion(size_t begin, size_t num) {
		assert_leq(begin+num, cur_);
		if(num < 2) return;
		std::sort(list_ + begin, list_ + begin + num);
	}

	/**
	 * Shuffle a portion of the list.
	 */
	void shufflePortion(size_t begin, size_t num, RandomSource& rnd) {
		assert_leq(begin+num, cur_);
		if(num < 2) return;
		size_t left = num;
		for(size_t i = begin; i < begin + num - 1; i++) {
            UINT32 rndi = rnd.nextU32() % left;
			if(rndi > 0) {
				std::swap(list_[i], list_[i + rndi]);
			}
			left--;
		}
	}

	/**
	 * Sort contents
	 */
	void sort() {
		sortPortion(0, cur_);
	}

	/**
	 * Return true iff every element is < its successor.  Only operator< is
	 * used.
	 */
	bool sorted() const {
		for(size_t i = 1; i < cur_; i++) {
			if(!(list_[i-1] < list_[i])) {
				return false;
			}
		}
		return true;
	}

	/**
	 * Delete element at position 'idx'; slide subsequent chars up.
	 */
	void remove(size_t idx) {
		assert_lt(idx, cur_);
		assert_gt(cur_, 0);
		for(size_t i = idx; i < cur_-1; i++) {
			list_[i] = list_[i+1];
		}
		cur_--;
	}

	/**
	 * Return a pointer to the beginning of the buffer.
	 */
	T *ptr() { return list_; }

	/**
	 * Return a const pointer to the beginning of the buffer.
	 */
	const T *ptr() const { return list_; }

	/**
	 * Set the memory category for this object.
	 */
	void setCat(int cat) {
		// What does it mean to set the category after the list_ is
		// already allocated?
		assert(null());
		assert_gt(cat, 0); cat_ = cat;
	}

	/**
	 * Return memory category.
	 */
	int cat() const { return cat_; }

	/**
	 * Perform a binary search for the first element that is not less
	 * than 'el'.  Return cur_ if all elements are less than el.
	 */
	size_t bsearchLoBound(const T& el) const {
		size_t hi = cur_;
		size_t lo = 0;
		while(true) {
			if(lo == hi) {
				return lo;
			}
			size_t mid = lo + ((hi-lo)>>1);
			assert_neq(mid, hi);
			if(list_[mid] < el) {
				if(lo == mid) {
					return hi;
				}
				lo = mid;
			} else {
				hi = mid;
			}
		}
	}
private:

	/**
	 * Initialize memory for EList.
	 */
	void lazyInit() {
		assert(list_ == NULL);
		list_ = alloc(sz_);
	}

	/**
	 * Initialize exactly the prescribed number of elements for EList.
	 */
	void lazyInitExact(size_t sz) {
		assert_gt(sz, 0);
		assert(list_ == NULL);
		sz_ = sz;
		list_ = alloc(sz);
	}

	/**
	 * Allocate a T array of length sz_ and store in list_.  Also,
	 * tally into the global memory tally.
	 */
	T *alloc(size_t sz) {
		T* tmp = new T[sz];
		assert(tmp != NULL);
		allocCat_ = cat_;
		return tmp;
	}

	/**
	 * Allocate a T array of length sz_ and store in list_.  Also,
	 * tally into the global memory tally.
	 */
	void free() {
		if(list_ != NULL) {
			assert_neq(-1, allocCat_);
			assert_eq(allocCat_, cat_);
			delete[] list_;
			list_ = NULL;
			sz_ = cur_ = 0;
		}
	}

	/**
	 * Expand the list_ buffer until it has at least 'thresh' elements.  Size
	 * increases quadratically with number of expansions.  Copy old contents
	 * into new buffer using operator=.
	 */
	void expandCopy(size_t thresh) {
		if(thresh <= sz_) return;
		size_t newsz = (sz_ * 2)+1;
		while(newsz < thresh) newsz *= 2;
		expandCopyExact(newsz);
	}

	/**
	 * Expand the list_ buffer until it has exactly 'newsz' elements.  Copy
	 * old contents into new buffer using operator=.
	 */
	void expandCopyExact(size_t newsz) {
		if(newsz <= sz_) return;
		T* tmp = alloc(newsz);
		assert(tmp != NULL);
		size_t cur = cur_;
		if(list_ != NULL) {
 			for(size_t i = 0; i < cur_; i++) {
				// Note: operator= is used
				tmp[i] = list_[i];
			}
			free();
		}
		list_ = tmp;
		sz_ = newsz;
		cur_ = cur;
	}

	/**
	 * Expand the list_ buffer until it has at least 'thresh' elements.
	 * Size increases quadratically with number of expansions.  Don't copy old
	 * contents into the new buffer.
	 */
	void expandNoCopy(size_t thresh) {
		assert(list_ != NULL);
		if(thresh <= sz_) return;
		size_t newsz = (sz_ * 2)+1;
		while(newsz < thresh) newsz *= 2;
		expandNoCopyExact(newsz);
	}

	/**
	 * Expand the list_ buffer until it has exactly 'newsz' elements.  Don't
	 * copy old contents into the new buffer.
	 */
	void expandNoCopyExact(size_t newsz) {
		assert(list_ != NULL);
		assert_gt(newsz, 0);
		free();
		T* tmp = alloc(newsz);
		assert(tmp != NULL);
		list_ = tmp;
		sz_ = newsz;
		assert_gt(sz_, 0);
	}
private:
	int cat_;      // memory category, for accounting purposes
	int allocCat_; // category at time of allocation
	T *list_;      // list pointer, returned from new[]
	size_t sz_;    // capacity
	size_t cur_;   // occupancy (AKA size)
};



} // end of namespace algn_ppbwt

#endif /* DS_H_ */
