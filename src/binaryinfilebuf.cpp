#include "binaryinfilebuf.h"

namespace barcodeSpace{
BinaryInFileBuf::BinaryInFileBuf(FILE* in)
{
    this->init();
    this->_pIn = in;
    if(!in){
        this->nultify();
        cerr<<classname()<<"invalid file handler!"<<endl;
        cerr.flush();
    }
    else
        this->load();
}
BinaryInFileBuf::BinaryInFileBuf(std::string file){
    this->init();
    this->_pIn = fopen(file.c_str(),"rb");
    if(!this->_pIn)
        throw std::runtime_error(file + " file is not invalid!\n");
    else
        this->load();
}
void BinaryInFileBuf::init(){
    this->_pIn = NULL;
    this->_closed = false;
    this->_done = false;
    this->_cur = 0;
    this->_end = 0;
}
void BinaryInFileBuf::nultify(){
    this->_closed = true;
    this->_done = true;
    this->_cur = 0;
    this->_end = 0;
}
void BinaryInFileBuf::close(){
    if(!this->_closed){
        fclose(this->_pIn);
        this->_done = true;
        this->_cur = 0;
        this->_end = 0;
        this->_closed = true;
    }
}
BinaryInFileBuf:: ~BinaryInFileBuf(){
        this->close();
}
void BinaryInFileBuf::setNewFile(FILE *in){

    if(!this->_closed)
        this->close();

    this->_pIn = in;
    if(!this->_pIn)
        nultify();
    else{
        this->_done = false;
        this->_closed = false;
        this->_cur = 0;
        this->load();
    }
}
size_t BinaryInFileBuf::gets(char* buf,size_t len){


    size_t store = 0;
    if(len && !this->eof()){
        while(this->_end - this->_cur < len && !_done){
            memcpy(buf + store,_buf + _cur,this->_end - this->_cur);
            store += (this->_end - this->_cur);
            len -= (this->_end - this->_cur);
            this->load();
            if(!this->_end)
                return store;
        }
        if(len){
            memcpy(buf + store,_buf+_cur,len);
            store += len;
            this->_cur += len;
        }
    }
    buf[store] = '\0';
    return store;

}
void BinaryInFileBuf::load(){
    if(!this->_closed){
        size_t sz = fread(_buf,1,BUF_SZ,this->_pIn);
        if(sz < BUF_SZ || feof(_pIn))
            this->_done = true;
        if(ferror(_pIn))
            throw std::runtime_error(classname() + "invalid file state when reading file");
        this->_end = sz;
        this->_cur = 0;
    }
}
void BinaryInFileBuf::brewind(){
    if(!this->_closed){
        memset(_buf,0,BUF_SZ);
        rewind(this->_pIn);
        this->_cur = 0;
        this->load();
    }

}
} //end of namespace
