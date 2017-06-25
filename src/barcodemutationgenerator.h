#ifndef BARCODEMUTATIONGENERATOR_H
#define BARCODEMUTATIONGENERATOR_H

#include "kmers_dictionary.h"
#include "typdefine.h"

#include <string>
#include <vector>

namespace barcodeSpace {
class BarcodeMutationGenerator
{
public:
    BarcodeMutationGenerator(size_t klen);

    void mutateCenterInplace(const kmer& k, std::vector<kmer>& mutated_center) {
        for (size_t i = 0; i <_mut_mask.size(); ++i) {
            mutated_center[i] = _mut_mask[i] ^ k;
        }
    }
    void mutateCenterInplace(const std::string& center, std::vector<std::string>& mutated_center) {
        for (size_t i = 0; i < center.length(); ++i) {
            std::string temp = center;
            for (size_t bp = 0; bp < 4; ++bp) {
                temp[i] = _dict->dna2asc(bp);
                mutated_center.push_back(temp);
            }
        }
    }
    std::vector<kmer> mutateCenter(const kmer& k) {
        std::vector<kmer> result;
        for (size_t i = 0; i <_mut_mask.size(); ++i) {
            result.push_back(_mut_mask[i] ^ k);
        }
        return result;
    }
    
    std::vector<std::string> mutateCenter(const std::string& center) {
        std::vector<std::string> result;
        for (size_t i = 0; i < center.length(); ++i) {
            std::string temp = center;
            for (size_t bp = 0; bp < 4; ++bp) {
                temp[i] = _dict->dna2asc(bp);
                result.push_back(temp);
            }
        }
        return result;
    }
    
private:
    void init();
private:
    size_t  _k_len;
    std::vector<kmer> _mut_mask;
    std::shared_ptr<kmersDictionary> _dict;
};
}
#endif // BARCODEMUTATIONGENERATOR_H
