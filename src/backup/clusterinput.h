#ifndef CLUSTERINPUT_H
#define CLUSTERINPUT_H
#include "suffix_commons.h"
#include <unordered_map>
#include "cluster.h"
#include "filebuf.h"
#include "util.h"
#include "kmers_bitwisetransform.h"
#include "dictatorcenter.h"
#include "dictatorcluster.h"
using std::unordered_map;
namespace prefixMatching{
class clusterInput
{
    typedef list<std::shared_ptr<cluster> >  Bin;
public:
    clusterInput(const std::string& filename);
    Bin& barcodes(int klen){return this->_bins[klen];}
    const std::unordered_map<int,Bin>&      bins()const{return this->_bins;}
private:
    void load();
private:
    std::string                                     _filename;
    std::unordered_map<int,Bin>                     _bins;
    std::unique_ptr<FileBuf>                        fb_;
    std::list<std::string>                          _head;

};
}
#endif // CLUSTERINPUT_H
