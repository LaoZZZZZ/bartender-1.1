#ifndef SINGLEENDREADER_H
#define SINGLEENDREADER_H
#include "suffix_commons.h"
#include "pattern.h"
#include "sequence.h"
namespace prefixMatching{
// This class loads sequence file and support different file format
// I think now we only support fastq and fasta formats.
class SingleEndReader
{
public:
    SingleEndReader(const string &seqfile,file_format format);
    // Returns true if reaches the end of the file.
    // If there is no valid read in the remaining file, return NULL.
    void nextRead(sequence* read,bool* done);
    int totalReads()const {return this->total_reads;}
private:
    void init();
    string seqfile_;  // sequence file
    file_format format_;    // file format
    int total_reads_;
    std::unique_ptr<patternParser> file_reader_;
};
}  // namespace prefixMatching
#endif // SINGLEENDREADER_H
