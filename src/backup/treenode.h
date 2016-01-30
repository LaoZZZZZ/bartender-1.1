#ifndef TREENODE_H
#define TREENODE_H
#include "suffix_commons.h"
#include <list>
using std::list;
namespace prefixMatching {
class treeNode{
public:
    treeNode(int id):_parent(NULL),_start(0),_end(0),_id(id){}
    ///////////////////////////////////////////////////
    /// \brief setter funcitons
    ///////////////////////////////////
    void setStart(UINT16 s){this->_start = s;}
    void setEnd(UINT16 e){this->_end = e;}
    void setAccess(int v, int index){ _access |= (v << index);}
    void setParent(treeNode* p){this->_parent = p;}
    void setID(int id){ this->_id = id;}
    ///////////////////////////////////////////////////////////////
    /// virtual functions
    /////////////////////////////////
    virtual void  setChild(treeNode*){
        return ;
    }
    virtual void setChild(treeNode*,int){
        return ;
    }

    virtual const treeNode*         child(size_t)const{throw 1;}
    virtual treeNode*         child(size_t ){throw 1;}
    virtual bool isLeaf()const{return true;}

    ////////////////////////////////////////////////////////////////
    ////////////////////getter function ////////////////////////////
    ///////////////////////////////////////////////////////////////
    ///
    UINT16  start()const{return this->_start;}
    UINT16  end()const{return this->_end;}
    int id()const{return _id;}
    const treeNode* parent()const{return this->_parent;}
    treeNode* parent(){return this->_parent;}
    bool access(int indx){ return  1&(_access>>indx);}
    virtual ~treeNode(){}
private:
    // the range is [_start,end), closed at left but open at right end
    treeNode*                               _parent;
    UINT16                                  _start;
    UINT16                                  _end;
    int                                     _id;  // sequence id
    UINT8                                   _access;
};
class internalNode : public treeNode
{
public:
//    internalNode();
    internalNode(int id):treeNode(id){
        memset(this->_child,0,5*sizeof(treeNode*));
    }

    ///////////////////////////////////////////////////////////////
    /// virtual functions
    /////////////////////////////////

    bool isLeaf()const{return false;}
    virtual void setChild(treeNode*p,int ind){
         this->_child[ind] = p;
    }
    void  setChild(treeNode* p){
            memcpy(&(this->_child),p,5*sizeof(treeNode*));
    }
    /**
      getter function
      */

    const treeNode*         operator[](size_t ind)const{assert(ind<=4);return this->_child[ind];}
    treeNode*         operator[](size_t ind){assert(ind<=4);return this->_child[ind];}
    const treeNode*         child(size_t ind)const{assert(ind<=4);return this->_child[ind];}
    treeNode*         child(size_t ind){assert(ind<=4);return this->_child[ind];}
    ~internalNode(){}
private:
    treeNode*                               _child[5];
};

class leafNode : public treeNode{
public:
    leafNode(int id):treeNode(id){
    }
    bool isLeaf()const{return true;}

};
}

#endif // TREENODE_H
