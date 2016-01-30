#include "distanceselector.h"


#include <iostream>
#include <math.h>

namespace barcodeSpace {
DistanceSelector::DistanceSelector(double error,double cut_off, size_t klen)
    : _error(error), _cutoff(cut_off), _klen(klen)
{
}
int DistanceSelector::calculateDistance(size_t max_size) {

    int distance = 1;
    for (int i = 1; i < _klen; ++i) {
        double value = max_size * pow(_error, i) * pow(1 - _error, _klen - i);
        if ( value > _cutoff) {
            distance = i;
        } else {
            break;
        }
    }
    return distance;
}

}   // namespace barcodeSpace
