#ifndef CLUSTERINPUT_H
#define CLUSTERINPUT_H

#include "cluster.h"
#include "filebuf.h"


#include <list>
#include <memory>
#include <string>
#include <unordered_map>

namespace barcodeSpace{
/**
  * Loads the clusters from the given file.
*/
class clusterInput
{
    typedef list<std::shared_ptr<cluster> >  Bin;
public:
    clusterInput(const std::string& filename);
    Bin& barcodes(int klen){return this->_bins[klen];}
    const std::unordered_map<size_t,Bin>&      bins()const{return this->_bins;}

private:
    void load();

private:
    std::string                                     _filename;
    std::unordered_map<size_t,Bin>                     _bins;
    std::unique_ptr<FileBuf>                        fb_;
    std::list<std::string>                          _head;
};
}
#endif // CLUSTERINPUT_H
