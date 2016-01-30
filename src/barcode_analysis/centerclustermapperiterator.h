#ifndef CENTERCLUSTERMAPPERITERATOR_H
#define CENTERCLUSTERMAPPERITERATOR_H

#include "centerclustermapper.h"

#include <list>
#include <memory>
namespace barcodeSpace {

// Iterates the CenterClusterMapper in a very simple way.
class CenterClusterMapperIterator
{
public:
    CenterClusterMapperIterator(const std::shared_ptr<CenterClusterMapper>& linker);
    bool has_next() {return findNextValid();}
    const std::shared_ptr<cluster>& next() {
        return _data->getClusterByID(_cur++);
    }
    void reset() {
        _cur = 0;
        findNextValid();
    }
    void reset(const std::shared_ptr<CenterClusterMapper>& linker) {
        this->_data = linker;
        this->init();
    }
private:
    bool findNextValid() {
        while (_cur < _size && _data->getClusterByID(_cur).get() == NULL) {
            ++_cur;
        }
        return _cur < _size;
    }
    void init() {
        if (this->_data.get()) {
            _size = this->_data->size();
        } else {
            _size = 0;
        }
        this->reset();
    }
    std::shared_ptr<CenterClusterMapper> _data;
    size_t  _cur;
    size_t  _size;
};
}   // namespace barcodeSpace
#endif // CENTERCLUSTERMAPPERITERATOR_H
