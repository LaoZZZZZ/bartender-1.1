#ifndef CSVREADER_H
#define CSVREADER_H

#include "minicsv.h"


#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace barcodeSpace {
// A csv text file reader.

class CSVReader
{
public:
    CSVReader(const std::string& filename, bool header = true);

    // Return true if could read next row.
    // otherwise the columns does not have valid iterms.
    template<typename T>
    bool nextRow(std::vector<T>* columns) {
        bool has_next = readNext(columns);
        if (has_next && !_header.empty()) {
            assert(columns->size() == _header.size()); 
        }
        return has_next;
    }
    const std::vector<std::string>& Header()const {return _header;}
    ~CSVReader() {
        _file_handler.close();
    }
private:
    template<typename T>
    bool readNext(std::vector<T>* columns) {
        bool has_next = _file_handler.read_line();
        if (has_next) {
            size_t num_items = _file_handler.num_of_delimiter() + 1;
            T temp;
            for (int i = 0; i < num_items; ++i) {
                _file_handler >> temp;
                columns->push_back(temp);
            }
        }
        return has_next;
    }    
    std::vector<std::string> _header;
    csv::ifstream   _file_handler;
};
}   // namespace barcodeSpace
#endif // CSVREADER_H
