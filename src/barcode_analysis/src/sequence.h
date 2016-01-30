#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <string>
#include <cassert>
namespace barcodeSpace{

// Represents a read.
// The read should at least have sequence and id.
// It might has quality sequence depending on the file format.
class Sequence
{
public:
    Sequence(const std::string& id, const char* seq, const std::string& qual);
    Sequence(const std::string& id, const std::string& seq, const std::string& qual);
    Sequence(){this->patFw.clear();}
    /// Return true iff the read (pair) is empty
    bool empty() const {
        return patFw.empty();
    }

    /// Returns length of the read in the buffer
    size_t length() const {
        return patFw.length();
    }
    void set(const std::string &id, const char* seq, const std::string &qual){
        this->init(id,seq,qual);
    }
    const std::string& fowardSeq()const{return this->patFw;}
    std::string& fowardSeq(){return this->patFw;}
    std::string& quality(){return this->qual;}
    const std::string& quality()const{return this->qual;}
    int  operator[](size_t pos)const{
        return this->patFw[pos];
    }
    const std::string &id() const {return this->id_;}
    std::string &id() {return this->id_;}
    void clear(){
        this->patFw.clear();
        this->qual.clear();
        this->id_.clear();
    }
    virtual ~Sequence(){}
private:
    void init(const std::string &id,
              const char* seq,
              const std::string &qual){
        this->patFw.assign(seq);
        this->qual.assign(qual.begin(), qual.end());
        this->id_.assign(id.begin(),id.end());
        assert(qual.length() == patFw.length());
    }
private:
    std::string                      id_;
    std::string                      patFw;            // forward-strand sequence
    std::string                      qual;
};
}
#endif // SEQUENCE_H
