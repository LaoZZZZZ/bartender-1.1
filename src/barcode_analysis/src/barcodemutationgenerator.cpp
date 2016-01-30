#include "barcodemutationgenerator.h"

namespace barcodeSpace {
BarcodeMutationGenerator::BarcodeMutationGenerator(size_t klen) : _k_len(klen)
{
    init();
}
void BarcodeMutationGenerator::init() {
    // generate the mask that update the changes to the target barcode.
    for (size_t i = 0; i < _k_len; ++i) {
        _mut_mask.push_back(0x1 << (2 * i));
        _mut_mask.push_back(0x2 << (2 * i));
        _mut_mask.push_back(0x3 << (2 * i));
    }

}
}   // namespace barcodeSpace
