#include "sequencereader.h"
#include <string>
#include <memory>
namespace prefixMatching {
SequenceReader::SequenceReader(const string &filename)
    :is_done_(false), first_(false),filename_(filename)
{
    init();
}
void SequenceReader::init() {
    FILE* _nakeHandler = fopen(this->filename_.c_str(),"r");
    if(!_nakeHandler){
        throw runtime_error(string("Can not open file ") + filename_);
    }
    fHandler_.reset(new FileBuf(_nakeHandler));
    is_done_ = fHandler_->eof();
}
}   // namespace prefixMatching
