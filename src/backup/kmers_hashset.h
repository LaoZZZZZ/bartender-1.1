#ifndef KMERS_HASHSET_H
#define KMERS_HASHSET_H

#include <functional>
#include <string>
#include <map>
#include <unordered_map>
#include "google/dense_hash_map"
#include "google/sparse_hash_map"
#include "typdefine.h"
using google::dense_hash_map;      // namespace where class lives by default
using google::sparse_hash_map;
using std::string;
using std::map;
using std::unordered_map;
namespace prefixMatching{
/*template<typename key, typename value,template<typename,typename> class>
class kmers_hashset : public Kmers
{
public:
    typedef typename container<key, value>::iterator iterator;
    typedef typename container<key, value>::const_iterator const_iterator;
public:
    kmers_hashset(){}
    inline int  count(key& k) { return _set.count(k);}
    iterator find(key& k)  {return _set.find(k);}
    const_iterator find(key& k)  const{return _set.find(k);}
    inline size_t size() {return _set.size();}
    inline bool empty() {return _set.empty();}
    inline void erase(key& k) {_set.erase(_set.find(k));}
    void clear() {_set.clear();}
    inline value&  operator[](key& k){ return _set[k];}
    inline iterator begin(){return _set.begin();}
    const_iterator begin()const{return _set.begin();}
    inline iterator end(){return _set.end();}
    const_iterator end()const{return _set.end();}
    virtual string classname(){return std::string("kmers_hashset");}
protected:
    container<key,value>           _set;
};*/
template <typename T>
T hash(const T& k) {return k;}

// simple hash adapter for types without pointers
template <typename T>
struct MurmurHasher {
    T operator()(const T& t) const {
        return hash(t);
    }
};
template <typename T>
struct eqstr
{
  bool operator()(T s1, T s2) const
  {
    return !(s1^s2);
  }
};
/**
  * a dense hash map container support basic operation for traditional map
  * this container is especially suitable for small dictionary that persistent in
  * the memory
  */

template<typename key,typename value>
class kmers_denseHash{

    //define the iterator as outside iterator
public:
    typedef typename google::dense_hash_map<key, value>::iterator iterator;
    typedef typename google::dense_hash_map<key, value>::const_iterator const_iterator;

public:
    struct eqstr
    {
      bool operator()(const key& s1, const key& s2) const
      {
        return (s1 == s2);
      }
    };
    kmers_denseHash(){
        //_set.set_empty_key(ERRORFLAG-1);
        //_set.set_deleted_key(ERRORFLAG);
    }
    int  count(key& k) { return _set.count(k);}
    iterator find(key& k)  {return _set.find(k);}
    const_iterator find(key& k)  const{return _set.find(k);}
    size_t size() {return _set.size();}
    bool empty() {return _set.empty();}
    void erase(key& k) {_set.erase(_set.find(k));}
    virtual void clear() {
        _set.clear();
    }
    value&  operator[](key& k){ return _set[k];}
    iterator begin(){return _set.begin();}
    const_iterator begin()const{return _set.begin();}
    iterator end(){return _set.end();}
    const_iterator end()const{return _set.end();}
    virtual string classname(){return std::string("kmers_hashset");}
protected:

    google::dense_hash_map<key, value,std::tr1::hash<key>, eqstr> _set;
};
class kmers_Goole_DenseMap : public kmers_denseHash<kmer,EList<UINT64> >{
public:
    kmers_Goole_DenseMap()
    {
        _set.set_empty_key(ERRORFLAG - 1);
        _set.set_deleted_key(ERRORFLAG);
    }
    void clear(){
        /*iterator itor;
        for(itor = _set.begin(); itor != _set.end(); itor++){
            (itor->second).clear();
        }*/
        _set.clear();
    }
};
#ifdef UNORDER
typedef kmers_Goole_DenseMap kmers_Uint64_DenseMap;
#else
typedef map<kmer,EList<UINT64> > kmers_Uint64_DenseMap;
#endif
template<typename key,typename value>
class kmers_sparseHash{

    //define the iterator as outside iterator
public:
    typedef typename google::sparse_hash_map<key, value>::iterator iterator;
    typedef typename google::sparse_hash_map<key, value>::const_iterator const_iterator;

public:
    kmers_sparseHash(){
    }
    int  count(const key& k) { return _set.count(k);}
    iterator find(const key& k)  {return _set.find(k);}
    const_iterator find(const key& k)  const{return _set.find(k);}
    size_t size() const{return _set.size();}
    bool empty() const{return _set.empty();}
    void erase(key& k) {_set.erase(_set.find(k));}
    void clear() {_set.clear();}
    value&  operator[](const key& k){ return _set[k];}
    iterator begin(){return _set.begin();}
    const_iterator begin()const{return _set.begin();}
    iterator end(){return _set.end();}
    const_iterator end()const{return _set.end();}
    virtual string classname(){return std::string("kmers_sparseHash");}
protected:
    google::sparse_hash_map<key, value> _set;

};
/*class kmers_Uint64_SparseMap : public kmers_sparseHash<UINT64,list<UINT64> >{
public:
    kmers_Uint64_SparseMap()
    {
        //_set.set_empty_key(ERRORFLAG-1);
        //_set.set_deleted_key(ERRORFLAG);
    }
};*/
} //end of namespace
#endif // KMERS_HASHSET_H
