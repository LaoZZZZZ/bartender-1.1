#include "kmers_binaryreader.h"
#include <memory.h>
#include <string>
#include "typdefine.h"

using std::string;
namespace barcodeSpace{
kmers_binaryReader::kmers_binaryReader(string filename)
    :_filename(filename),_isopen(false)
{
    try{
        this->open();
    }catch(const exception& e){
        cerr<<e.what()<<endl;
        cerr.flush();
        exit(0);
    }
}
void kmers_binaryReader::read(hashSet& set){
    try{
        if(!this->_isopen){
            this->open();
        }else
            this->_pin->brewind();
        this->disk_read(set);
    }catch(const exception& e){
        cerr<<e.what()<<endl;
        cerr.flush();
        exit(0);
    }
}
void kmers_binaryReader::open(){
    try{
        if(this->_isopen)
            return;
        if(this->_filename.empty())
            throw runtime_error(classname() + "invalid frequency files name which is " + _filename + "!\n");
        FILE* f;
        f = fopen(this->_filename.c_str(),"rb");
        this->_pin = new BinaryInFileBuf(f);
        if(!this->_pin)
            throw runtime_error(classname() +  "can not open the frequency file which is " + _filename + "!\n");
        this->_isopen = true;
    }catch(const exception& e){
        cerr<<e.what()<<endl;
        cerr.flush();
        exit(0);

    }
}
void kmers_binaryReader::disk_read(hashSet &set){
    const size_t lk = sizeof(kmer);
    const size_t lf = sizeof(freq);
    const size_t len = lk + lf;
    char* buf = new char[len + 1];
    memset(buf,0,len + 1);
    while(!this->_pin->eof()){
        size_t al = this->_pin->gets(buf,len);
        if(al != len)
            throw runtime_error(classname() + "invalid binary file which is " + _filename + "!\n" );
        kmer m = 0;
        memcpy(&m,buf,lk);
        freq f = 0;
        memcpy(&f,buf + lk,lf);
        set[m] = f;
        memset(buf,0,len+1);
    }
}
}
