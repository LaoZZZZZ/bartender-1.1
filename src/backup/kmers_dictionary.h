#ifndef KMERS_DICTIONARY_H
#define KMERS_DICTIONARY_H
#include "suffix_commons.h"
#include <unordered_map>
using std::unordered_map;
namespace prefixMatching{
/**
 * holds all possible characters kmers support
 * basically contains ATCGN
 * which coded separatedly as 0 1 2 3 4
*/
    class kmers_Dictionary
    {
    public:
        virtual string classname();
        static kmers_Dictionary* getInstance();
        int getCharNum(char c);
        int operator[](char c);
        int numAt(char c);
        char charAt(unsigned index);
        string characterSet();
    protected:
        kmers_Dictionary();
        bool isValid(char c);
    private:
        static kmers_Dictionary*                _pIns;
        std::unordered_map<char,int>            _dict;
        char                                    _charSet[4];
    };
} //namespace kmers_ppseq
#endif // KMERS_DICTIONARY_H
