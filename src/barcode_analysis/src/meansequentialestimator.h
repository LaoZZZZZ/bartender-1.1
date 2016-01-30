#ifndef MEANESTIMATOR_H
#define MEANESTIMATOR_H

#include "typdefine.h"
namespace barcodeSpace {
// Estimate the mean of group of cluster size
// This class could avoid overflow by using on-line algorithm.
class MeanSequentialEstimator
{
public:
    MeanSequentialEstimator() : mean(0.0), count(0) {}
    void  Add(int sz);
    double GetMean()const {return mean;}
private:
    double mean;
    int count;
};
}   // namespace barcodeSpace
#endif // MEANESTIMATOR_H
