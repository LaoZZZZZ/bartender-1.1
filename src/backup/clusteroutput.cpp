#include "clusteroutput.h"

namespace prefixMatching{
clusterOutput::clusterOutput(std::string filename,const vector<std::string>& colums,bool head):
                              _filename(filename),_out(filename,std::fstream::out),
                              _columns(colums),_head(head)
{
    this->init();
}
void clusterOutput::init(){
    if(!this->_out.is_open())
        throw std::runtime_error(string("can not open file ") + this->_filename);
    if(_head){
        // write the head
        this->_out<<"center"<<',';
        this->_out<<this->_columns.front();
        auto iter = this->_columns.begin();
        advance(iter,1);
        while(iter != this->_columns.end()){
            this->_out<<','<<*iter;
        }
        this->_out<<std::endl;
    }
}
bool clusterOutput::write(const list<std::shared_ptr<cluster>>& clusters){
    for(const shared_ptr<cluster>& c: clusters){
        assert(c->columnSize() == this->_columns.size());
        this->_out<<*(c.get())<<endl;
    }
    return true;
}

}
