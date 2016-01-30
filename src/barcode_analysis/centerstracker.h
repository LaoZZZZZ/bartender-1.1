#ifndef CENTERSTRACKER_H
#define CENTERSTRACKER_H

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

namespace barcodeSpace {
// Used to track the number of clusters that have
// different number of centers. The histogram
// i.e.
class CentersTracker
{
public:
    CentersTracker(size_t maximum_centers) : _maximum_centers(maximum_centers), _center_log(maximum_centers+1, 0) {

    }
    CentersTracker(const CentersTracker& cp) {
        _center_log.assign(cp.Counts().begin(), cp.Counts().end());
        _maximum_centers = _center_log.size();
    }
    void AddCount(size_t center_size) {
        if (center_size > _maximum_centers) {
            _center_log.back() += 1;
        } else {
            _center_log[center_size - 1] += 1;
        }
    }
    const std::vector<size_t>& Counts() const {
        return this->_center_log;
    }
    size_t TotalCenters() const {
        return std::accumulate(_center_log.begin(), _center_log.end(), 0);
    }
private:
    size_t _maximum_centers;
    std::vector<size_t> _center_log;
};
}   // namespace barcodeSpace
#endif // CENTERSTRACKER_H
