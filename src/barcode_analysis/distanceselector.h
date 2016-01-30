#ifndef DISTANCESELECTOR_H
#define DISTANCESELECTOR_H

#include "typdefine.h"

#include <cassert>
namespace barcodeSpace {
class DistanceSelector
{
public:
    DistanceSelector(double error, double threshold, size_t klen);
    int calculateDistance(size_t max_size);
private:
    int choose(size_t n, size_t k){
        assert(n >= k);
        assert(k > 0);
        int result = 1;
        for(size_t m = n; m > n - k; --m) {
            result *= m;
        }
        int denom = 1;
        for (size_t m = 2; m <= k; ++m) {
            denom *= m;
        }
        return result/denom;
    }
    double  _error;
    double _cutoff;
    size_t _klen;
};
}
#endif // DISTANCESELECTOR_H
