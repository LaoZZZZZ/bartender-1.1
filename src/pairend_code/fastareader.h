#ifndef FASTAREADER_H
#define FASTAREADER_H
#include "filebuf.h"
#include "sequence.h"
#include "sequencereader.h"
#include <memory>
namespace prefixMatching{
// This class is designed to load fasta sequence file
//
class FastaReader : public SequenceReader
{
public:
    FastaReader(const string &filename);
    void NextRead(Sequence& r, bool& succss, bool& done);
    static SequenceReader* CreateSequenceReader(const string& filename){
        return new FastaReader(filename);
    }
};
}
#endif // FASTAREADER_H
