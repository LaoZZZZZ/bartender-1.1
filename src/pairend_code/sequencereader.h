#ifndef SEQUENCEREADER_H
#define SEQUENCEREADER_H
#include <string>
#include <memory>
#include <functional>
#include "sequence.h"
#include "filebuf.h"
#include "formats.h"
namespace prefixMatching {
// An interface to load the sequence file with
// different file format,i.e FASTQ, FASTA
class SequenceReader
{
public:
    SequenceReader(const string &filename);
    bool IsDone() const {return is_done_;}
    virtual void NextRead(Sequence& r,
                          bool& success,
                          bool& done) = 0;
    virtual ~SequenceReader(){}
protected:
    std::unique_ptr<FileBuf> fHandler_;
    bool is_done_;
    bool first_;
private:
    void init();
private:
    string filename_;

};
}   // namespace prefixMatching
#endif // SEQUENCEREADER_H
