#ifndef CSVOUTPUT_H
#define CSVOUTPUT_H

#include "cluster.h"
#include "typdefine.h"

#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>
#include <memory.h>
#include <list>
#include <fstream>

namespace barcodeSpace{
/**
  * Self writtened CSV file format dumper.
  * It has the same functionality with class TableDumper.
  * The only difference is that this class use a buffer.
*/
template <typename T>
class CSVOutput
{
public:
    CSVOutput(const std::string& filename):
    _filename(filename),_out(filename,std::fstream::out)
    {
        this->init();
    }
    void Write(const std::vector<T>& columns) {
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
    void Write(const std::list<T>& columns){
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
    void Close() {
        this->_out.close();
    }
    ~CSVOutput(){
        this->_out.close();
    }

private:
    CSVOutput(const CSVOutput&);
    CSVOutput& operator=(const CSVOutput&);
    void init(){
        if(!this->_out.is_open())
            throw std::runtime_error(std::string("Could not open file ") + this->_filename);
        memset(buffer_, 0, BUF_SZ * sizeof(char));
        _out.rdbuf()->pubsetbuf(buffer_, BUF_SZ);
        
    }
    std::string                     _filename;
    
    // TODO(lu): Mannully buffer the stream to improve the speed.
    // This should be a minor issue currently.
    // Come back if there is extra time for this.
    const static size_t BUF_SZ = 128*1024;
    char    buffer_[BUF_SZ];

    std::ofstream                    _out;
};
}
#endif // CSVOUTPUT_H
