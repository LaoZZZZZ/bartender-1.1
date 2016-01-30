#ifndef FASTQREADER_H
#define FASTQREADER_H

#include <memory>
#include "sequencereader.h"
#include "sequence.h"
#include "filebuf.h"

namespace prefixMatching {
// Fastq file reader
class FastqReader : public SequenceReader
{
public:
    FastqReader(const string &filename);
    void NextRead(Sequence& r,bool& success, bool& done);
    static SequenceReader* CreateSequenceReader(const string& filename){
        return new FastqReader(filename);
    }
    ~FastqReader(){}
};
}   // namespace prefixMatching
#endif // FASTQREADER_H
