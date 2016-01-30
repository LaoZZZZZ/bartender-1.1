#include "barcodeextractor.h"

namespace prefixMatching{
barcodeExtractor::barcodeExtractor(std::string filename,const boost::regex& pat,
                                   file_format format,double qualThres):
                                    _readfile(filename),_pattern(pat),
                                    _seqFileHandler(NULL),_totalReads(0),_totalValid(0),
                                    _format(format),_qualThres(qualThres)
{
    this->init();
    this->extract();
}
void barcodeExtractor::init(){
    patternParser* parser = NULL;
    if(this->_format == FASTQ){
        parser = new fastqPattern(this->_readfile,this->_pattern,this->_qualThres);
    }else
        throw runtime_error("The given file format is not supported!\n");
    this->_seqFileHandler.reset(parser);

}
void barcodeExtractor::extract(){
    bool done(false);
    bool success(false);
    std::shared_ptr<sequence> read;
    string r_id;
    while(!done){
        this->_seqFileHandler->parse(r_id,read,success,done);
        if(success){
            size_t len = read->fowardSeq().length();
            if(0== this->_stats.count(len)){
                barcodeCounting* temp = new barcodeCounting(len);
                //stats.insert(std::make_pair<size_t,barcodeCounting*>(seq.length,temp));
                this->_stats[len] = std::shared_ptr<barcodeCounting>(temp);
                assert(temp);
                }
            this->_stats[len]->insert(read->fowardSeq());
            this->_totalValid++;
        }

        this->_totalReads++;
        r_id.clear();
    }

}
barcodeExtractor::~barcodeExtractor(){

}
}
