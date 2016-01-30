#ifndef IDGENERATOR_H
#define IDGENERATOR_H
#include <vector>
#include <algorithm>
#include <set>
namespace barcodeSpace {
// Given a list of takened id.
// this class could give the next smallest available id.
// This class is designed to make the id system as small as possible.
class IDGenerator
{
public:
    IDGenerator(const std::set<int>& taken_id);
    int nextID() {
        if (_cur >= _available.size()) {
            return _next++;
        } else {
            return _available[_cur++];
        }
    }
    bool isAvailable(int id) {
        return std::binary_search(_available.begin() + _cur, _available.end(), id);
    }
private:
    void init(const std::set<int>& taken);
    std::vector<int>    _available;
    int _next;
    size_t _cur;
};
}   // namespace barcodeSpace
#endif // IDGENERATOR_H
