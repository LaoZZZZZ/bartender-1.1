#include "util.h"

namespace prefixMatching{

std::list<std::string> strSplit(const std::string& str){
    string tmp;
    std::list<std::string> result;
    int len(str.length());
    for(int i = 0; i < len; i++){
        if(str[i] == ','){
            result.push_back(tmp);
            tmp.clear();
        }else{
            tmp += str[i];
        }
    }
    if(tmp.length())
        result.push_back(tmp);
    return result;
}
void strSplit(const std::string& str,std::list<std::string>& result){
    string tmp;
    result.clear();
    //std::list<std::string> result;
    int len(str.length());
    for(int i = 0; i < len; i++){
        if(str[i] == ','){
            result.push_back(tmp);
            tmp.clear();
        }else{
            tmp += str[i];
        }
    }
    if(tmp.length())
        result.push_back(tmp);
}

std::string reverseComplement(const std::string& seq){

    std::string result;
    size_t len = seq.length();
    while(len > 0){
        if(toupper(seq[len-1]) == 'A'){
            result += 'T';
        }else if(toupper(seq[len-1]) == 'C'){
            result += 'G';
        }else if(toupper(seq[len-1]) == 'G'){
            result += 'C';
        }else{
            result += 'A';
        }
        len--;
    }
    return result;
}
size_t kmerDistance(const kmer& k1,const kmer& k2,size_t klen){
    size_t total = 0;
    kmer sim = k1^k2;
    for(size_t i = 0; i < klen; i++){
        total += ((3 &sim >> (2*i)) > 0);
    }
    return total;
}
double similarityScore(const kmer& k,const list<kmers_freq>& barcodes,size_t klen){

    double result(0);
    for(auto it = barcodes.begin(); it != barcodes.end(); it++){
        result += kmerDistance(it->_key,k,klen)*it->_freq;
    }
    return result;
}
void outputBarcode(std::string outfile,const kmers_sparseHash<kmer,freq>& barcode){
    BinaryOutFileBuf* naked = new BinaryOutFileBuf(outfile.c_str());
    std::shared_ptr<BinaryOutFileBuf> handler(naked);
    for(auto iter = barcode.begin(); iter != barcode.end(); iter++){
        handler->write(iter->first,8);
        handler->write(iter->second,4);
    }
}
/**
  * Load barcode table from the file
*/
void loadBarcode(std::string infile,kmers_sparseHash<kmer,freq>& barcode){

    BinaryInFileBuf* naked = new BinaryInFileBuf(infile);
    std::shared_ptr<BinaryInFileBuf> handler(naked);
    char buf[13];
    memset(buf,0,13);
    kmer k(0);
    freq f(0);
    while(!handler->eof()){
        handler->gets(buf,12);
        memcpy(&k,buf,8);
        memcpy(&f,buf+8,4);
        barcode[k] = f;
    }

}
void outputCluster(std::fstream& handler,std::shared_ptr<cluster>& c){

}
}
