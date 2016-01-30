#ifndef BARCODEMUTATIONGENERATOR_H
#define BARCODEMUTATIONGENERATOR_H

#include "typdefine.h"

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
    std::vector<kmer> mutateCenter(const kmer& k) {
        std::vector<kmer> result;
        for (size_t i = 0; i <_mut_mask.size(); ++i) {
            result.push_back(_mut_mask[i] ^ k);
        }
        return result;
    }
private:

    void init();
    size_t  _k_len;
    std::vector<kmer> _mut_mask;
};
}
#endif // BARCODEMUTATIONGENERATOR_H
