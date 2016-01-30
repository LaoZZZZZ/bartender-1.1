#include "kmers_dictionary.h"

namespace prefixMatching{
    kmers_Dictionary* kmers_Dictionary::_pIns = NULL;
    kmers_Dictionary::kmers_Dictionary()
    {
        _dict.clear();
        _dict.insert(make_pair('A',0));
        _dict.insert(make_pair('a',0));
        _dict.insert(make_pair('C',1));
        _dict.insert(make_pair('c',1));
        _dict.insert(make_pair('G',2));
        _dict.insert(make_pair('g',2));
        _dict.insert(make_pair('T',3));
        _dict.insert(make_pair('t',3));
        _dict.insert(make_pair('N',4));
        _dict.insert(make_pair('n',4));
        this->_charSet[0] = 'A';
        this->_charSet[1] = 'C';
        this->_charSet[2] = 'G';
        this->_charSet[3] = 'T';
        assert_eq(_dict.size(),10);
    }
    string kmers_Dictionary::classname(){
        return std::string("class::kmers_Dictionary");
    }
    kmers_Dictionary* kmers_Dictionary::getInstance(){
        if(!_pIns){
            _pIns = new kmers_Dictionary();
        }
        assert(_pIns);
        return _pIns;
    }
    bool kmers_Dictionary::isValid(char c) {
        return this->_dict.count(c);
    }
    int kmers_Dictionary::getCharNum(char c) {

        if(isValid(c)){
            return _dict[c];
        }
         else{
            stringstream s;
            s<<"invalid character in the input character ";
            s<<c;
            s<<", kmers_ppseq ONLY supports sequence that ONLY contains ATCG or atcg";
            throw logic_error(s.str());
            }

    }

    int kmers_Dictionary::operator[](char c){
        return this->getCharNum(c);
    }
    int kmers_Dictionary::numAt(char c){
        return this->getCharNum(c);
    }
    char kmers_Dictionary::charAt(unsigned index){
        if(index >= 4){
            stringstream str;
            str <<"invalid index which could only within 0-3 while the given index is ";
            str <<index;
            throw out_of_range(str.str());
        }
        return this->_charSet[index];
    }
    string kmers_Dictionary::characterSet(){
        stringstream f;
        for(auto itor = _dict.begin();itor != _dict.end();itor++){
            f<<itor->first;
        }
        return f.str();
    }
}
