#include "idgenerator.h"

#include <algorithm>
#include <set>
using std::set;
namespace  barcodeSpace {

IDGenerator::IDGenerator(const std::set<int>& takened_id)
{
    this->init(takened_id);
}

void IDGenerator::init(const std::set<int>& taken) {
    _available.clear();
    if (taken.empty()) {
        _next = 0;
    }
    else {
        int maxid = *taken.rbegin();
        for (int i = 0; i < maxid; ++i) {
            if (taken.find(i) == taken.end()) {
                _available.push_back(i);
            }
        }
        _next = maxid + 1;
    }
    _cur = 0;
}
}   // namespace barcodeSpace
