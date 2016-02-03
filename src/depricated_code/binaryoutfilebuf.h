#ifndef BINARYOUTFILEBUF_H
#define BINARYOUTFILEBUF_H
#include "typdefine.h"
#include "suffix_commons.h"
namespace barcodeSpace{
/**
 * Wrapper for a buffered output stream that writes bitpairs.
 */
class BinaryOutFileBuf {
public:
    /**
     * Open a new output stream to a file with given name.
     */
    BinaryOutFileBuf(const char *in);
    /**
     * write a byte to the buffer
     */
    void writeUint8(UINT8 d);
    /**
     * Write a 8 byte into the buf.
     * Flush the buffer if it's full.
     */
    void writeUint32(uint32_t bp);
    /**
     * Write a single bitpair into the buf.  Flush the buffer if it's
     * full.
     */
    void writeUint64(UINT64 bp);
    /**
      * \brief write a type of variable with length of len
      */
    template<typename T>
    void write(T v,size_t len){

            //throw runtime_error("invalid file position\n");
        if(closed_)
            throw std::runtime_error("Error: trying to wirte data to a already closed file");
        if(len > BUF_SZ){
            flush();
            if(!fwrite((const void *)(&v), len, 1, out_)) {
                throw std::runtime_error("Error writing to ultimate output file!");
            }
        }
        else if(cur_ >= BUF_SZ-len + 1) {
            flush();
        }
        memcpy(buf_ + cur_,&v,len);
        cur_ += len;
        if(cur_ == BUF_SZ) {
            flush();
        }
    }
    /**
      * write char-string
     */
    void writeChars(const char* buf,size_t len);
    /**
      * flush the mem part to file
     */
    void flush();
   /**
     * Write any remaining bitpairs and then close the input
     */
    void close() {
        if(!closed_){
            flush();
            fclose(out_);
            closed_ = true;
        }
    }
    /**
      * return the postion right after the last valid character
      */
    long int tellp(){
        if(closed_)
            return -1L;
        else{
            return (pos_ + cur_);
           // return ftell(out_);
        }
    }
    /**
      * write a value to the file right at pos
      */
    template<typename T>
    void writep(T& v,int len,size_t pos){
        if(pos < pos_){
            fseek(out_,pos,SEEK_SET);
            fwrite(&v,len,1,out_);
           // fseek(out_,pos_,SEEK_SET);
        }
        else if(pos >= pos_ && (pos+len) <= tellp()){
            for(int i = 0; i < len; i++){
                buf_[pos - pos_ + i] &= 0;
                buf_[pos - pos_ + i] |= v >>((len -1 - i)*8);
            }
        }
        else{
            cerr<<"the position is out of the file range!"<<endl;
            cerr.flush();
            exit(-1);
        }
    }
    ~BinaryOutFileBuf(){
        if(!closed_){
            flush();
            if(out_)
                fclose(out_);
            closed_ = true;
        }
        //delete []buf_;
    }
    std::string classname(){return std::string("BinaryOutFileBuf");}
    static UINT32  memConsume(){ return BUF_SZ;}
private:
    static const size_t BUF_SZ = 128 * 1024;
    FILE     *out_;
    UINT32   cur_;
    UINT8    buf_[BUF_SZ]; // (large) input buffer
    bool     closed_;
    long     pos_;
};  //end of class
} //end of namespace
#endif // BINARYOUTFILEBUF_H
