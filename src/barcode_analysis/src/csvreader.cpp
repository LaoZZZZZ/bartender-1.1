#include "csvreader.h"

#include <stdexcept>
#include <string>

using std::string;

namespace barcodeSpace {

CSVReader::CSVReader(const std::string& filename, bool header):
     _file_handler(filename.c_str())
{
    _file_handler.set_delimiter(',', "");
    if (!_file_handler.is_open()) {
        throw std::runtime_error(string("Could not open file ") + filename);
    }
    if (header) {
        readNext(&_header);
    }
}

}   // namespace prefixmatching
