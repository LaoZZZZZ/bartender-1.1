#ifndef CSVREADER_H
#define CSVREADER_H

#include "minicsv.h"

#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

namespace barcodeSpace {
// A csv text file reader.

template<typename T>
class CSVReader
{
public:
    CSVReader(const std::string& filename, bool header):
    _file_handler(filename.c_str()), _filename(filename), _total_line(0)
    {
        _file_handler.set_delimiter(',', "");
        if (!_file_handler.is_open()) {
            throw std::runtime_error(std::string("Could not open file ") + filename);
        }
        if (header) {
            if (!readHeader()) {
                throw std::runtime_error(std::string("There is no head in the file ") + filename);
            }
        }
        readNext();
    }
    // Return true if could read next row.
    // otherwise the columns does not have valid iterms.
    bool nextRow(std::vector<T>* columns) {
        if (_cache.empty()) {
            return false;
        }
        columns->assign(_cache.begin(), _cache.end());
        readNext();
        return true;
    }
    void skip() {
        readNext();
    }
    bool peek(std::vector<T>& columns) {
        columns.assign(_cache.begin(), _cache.end());
        return !_cache.empty();
    }
    const std::vector<std::string>& Header()const {return _header;}
    ~CSVReader() {
        if (!_cache.empty()) {
            std::cerr << "Haven't reached the end of the csv file: " << _filename << std::endl;
        }
        _file_handler.close();
    }
    size_t TotalRecord() const { return _total_line;}
private:
    bool readNext() {
        _cache.clear();
        bool has_next = _file_handler.read_line();
        if (has_next) {
            size_t num_items = _file_handler.num_of_delimiter() + 1;
            T temp;
            for (int i = 0; i < num_items; ++i) {
                _file_handler >> temp;
                _cache.push_back(temp);
            }
            _total_line += 1;
        }
        return has_next;
    }
    
    bool readHeader() {
        bool has_next = _file_handler.read_line();
        if (has_next) {
            size_t num_items = _file_handler.num_of_delimiter() + 1;
            std::string temp;
            for (int i = 0; i < num_items; ++i) {
                _file_handler >> temp;
                _header.push_back(temp);
            }
        }
        return has_next;
    }
    
    std::vector<std::string> _header;
    std::vector<T>  _cache;
    csv::ifstream   _file_handler;
    std::string     _filename;
    size_t  _total_line;
};
}   // namespace barcodeSpace
#endif // CSVREADER_H
