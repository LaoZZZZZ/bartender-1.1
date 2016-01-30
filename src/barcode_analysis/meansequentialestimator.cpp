#include "meansequentialestimator.h"


namespace barcodeSpace {
void  MeanSequentialEstimator::Add(int sz) {
    ++count;
    mean = mean + (sz - mean)/count;
}

}
