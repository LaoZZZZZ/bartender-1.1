#include "pattern.h"
#include "fastapattern.h"
#include "fastqpattern.h"
#include "formats.h"

#include <string>
#include <stdexcept>
using std::string;

namespace barcodeSpace {

patternParser* CreatePatternParser(const string &filename, file_format format) {
    if (format == FASTQ) {
        return new fastqPattern(filename);
    } else if (format == FASTA) {
        return new fastaPattern(filename);
    } else {
        throw std::runtime_error("Does not support the specified file format!\n It looks like the input raw reads file is not either FASTQ or FASTA!\n");
    }
}

}   // namespace barcodeSpace
