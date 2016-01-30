#include "binaryoutfilebuf.h"

namespace prefixMatching{
    /**
     * Open a new output stream to a file with given name.
     */
    BinaryOutFileBuf:: BinaryOutFileBuf(const char *in) : out_(NULL),cur_(0),
        closed_(true){
        if(in == NULL)
            throw invalid_argument("invalid output file name!");
        out_ = fopen(in, "wb");
        if(out_ == NULL) {
            throw std::runtime_error(std::string("can not open ") + std::string(in));
        }
        memset(buf_, 0, BUF_SZ);
        closed_ = false;
        pos_    = 0;
    }
/**
 * write a byte to the buffer
 */
void BinaryOutFileBuf::writeUint8(UINT8 d){

    if(closed_)
        throw std::runtime_error("Error: trying to write data to a already closed file");
    buf_[cur_] |= d;
    cur_++;
    if(cur_ >= BUF_SZ) {
        flush();
    }

}

/**
 * Write a 8 byte into the buf.
 * Flush the buffer if it's full.
 */
void BinaryOutFileBuf::writeUint32(uint32_t bp) {
    if(closed_)
        throw std::runtime_error("Error: trying to wirte data to a already closed file");
    if(cur_ >= BUF_SZ-3) {
        flush();
    }
    memcpy(buf_+cur_,&bp,4);
    this->cur_ += 4;
    if(cur_ == BUF_SZ) {
        flush();
    }
}
/**
 * Write a single bitpair into the buf.  Flush the buffer if it's
 * full.
 */
void BinaryOutFileBuf::writeUint64(UINT64 bp) {
    if(closed_)
        throw std::runtime_error("Error: trying to wirte data to a already closed file");
    if(cur_ >= BUF_SZ-7) {
        flush();
    }
    memcpy(buf_+cur_,&bp,8);
    this->cur_ += 8;
    if(cur_ == BUF_SZ) {
        flush();
    }
}

/**
  * write char-string
 */
void BinaryOutFileBuf::writeChars(const char* buf,size_t len){
    try{
        if(closed_)
            throw std::runtime_error("Error: trying to wirte "\
                                "data to a already closed file");
        if(len > BUF_SZ){
            flush();
            fwrite(buf,1,len,out_);
        }else{
            if(cur_ >= BUF_SZ-len + 1) {
                flush();
            }
            memcpy(buf_ + cur_,buf,len);
            cur_ += len;
        }
        if(cur_ == BUF_SZ) {
            flush();
        }
    }catch(const exception& e){
        cerr<<e.what()<<endl;
        cerr.flush();
        exit(-1);
    }


}

/**
  * flush the mem part to file
 */
void BinaryOutFileBuf::flush(){
    try{
        if(closed_)
            throw std::runtime_error("Error: trying to wirte"\
                                "data to a already closed file when flushing");
        if(cur_ == 0)
            return;
        else{
            if(!fwrite((const void *)buf_, cur_, 1, out_)) {
                throw std::runtime_error("Error writing to ultimate output file!");
            }
            pos_ = ftell(out_);
            // Reset to beginning of the buffer
            cur_ = 0;
            memset(buf_,0,BUF_SZ);
        }
    }catch(const exception& e){
        cerr<<e.what()<<endl;
        cerr.flush();
        exit(-1);
    }
}


} //end of namespace
