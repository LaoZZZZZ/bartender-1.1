#include "csvoutput.h"
#include "typdefine.h"

#include <list>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <memory.h>
using std::vector;
using std::list;
using std::shared_ptr;
using std::runtime_error;
using std::string;
namespace barcodeSpace{
CSVOutput::CSVOutput(const std::string& filename,const list<std::string>& colums):
                      _filename(filename),_out(filename,std::fstream::out),
                      _columns(colums.begin(), colums.end())
{
    this->init();
}

void CSVOutput::init(){
    if(!this->_out.is_open())
        throw std::runtime_error(std::string("Could not open file ") + this->_filename);
    memset(buffer_, 0, BUF_SZ);
    _out.rdbuf()->pubsetbuf(buffer_, BUF_SZ);
    if(!this->_columns.empty()){

        // Write the header
        this->_out << this->_columns.front();
        auto iter = this->_columns.begin();
        advance(iter,1);
        while(iter != this->_columns.end()){
            this->_out<<','<<*iter;
            advance(iter, 1);
        }
        this->_out << std::endl;
    }
}

void CSVOutput::Write(const std::list<kmer>& columns){
    if (columns.empty()) return;
    this->_out << columns.front();
    auto iter = columns.begin();
    advance(iter, 1);
    while (iter != columns.end()) {
        this->_out << "," << *iter;
        advance(iter, 1);
    }
    this->_out << std::endl;
}

}
