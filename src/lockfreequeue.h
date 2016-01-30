#pragma once

#include <list>
#include <boost/lockfree/queue.hpp>

using boost::lockfree::queue;
using std::list;
template<typename T>
class lockFreeQueue{
public:
	lockFreeQueue(){}
    lockFreeQueue(size_t limit) :_que(limit){}
    void   add(T item){
        while(!this->try_push(item));
    }
    T   remove(){
        T r;
        while(!this->try_pop(r));
        return r;
    }
    bool tryAdd(T item){
        return this->try_push(item);
    }
    bool try_remove(T& r){
        return this->try_pop(r);
    }
    /**
      * try to add a batch of elements to the queue
      * if the queue reach the threshold, then the current
      * thread block for next turn
    */
     bool  tryAdd(std::list<T>&  elems){

         while(!elems.empty()){
             if(!this->try_push(elems.back()))
                     return false;
             elems.pop_back();
         }
         return true;
    }
     /**
       *  try to get maximum batch number of elems form the queue
       *  return true upon success otherwise false
       */
     bool   try_remove(list<T>& elems,size_t& batch){
         size_t it = 0;
         while(it < batch){
             T tmp;
             if(!this->try_pop(tmp))
                 return it > 0;
             else{
                elems.push_back(tmp);
             }
             it ++;
         }
         return true;
     }
	/**
	  * \@brief thread-safe status check. But if multiple threads
	  * are changing the queue, this status should not be used as logic
	*/
    bool empty(){ return this->_que.empty(); }
    ~lockFreeQueue(){}
private:
    /**
     * \@brief  non-waiting pop
    */
    bool try_pop(T& r){
        return this->_que.pop(r);
    }

    /**
     * \@brief non-waiting push
    */
    bool try_push(const T& e){
        return this->_que.push(e);
    }
private:
	boost::lockfree::queue<T, boost::lockfree::fixed_sized<true> >	    _que;
};
