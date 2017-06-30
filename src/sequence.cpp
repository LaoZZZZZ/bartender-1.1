#include "sequence.h"
#include "kmers_bitwisetransform.h"
#include <string>
using std::string;
namespace barcodeSpace{
Sequence::Sequence(const string &id,
                   const char *seq,
                   const string &qual)
{
    this->init(id,seq,qual);
}
Sequence::Sequence(const string& id,
                   const string& seq,
                   const string& qual)
{
    this->init(id,seq.c_str(),qual);
}
}
