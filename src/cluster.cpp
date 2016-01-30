#include "cluster.h"
#include "kmers_bitwisetransform.h"
#include "kmerdecoder.hpp"
//#include "kmers_dictionary.h"
#include "typdefine.h"


#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <array>

using namespace std;
namespace barcodeSpace {

int cluster::ClusterCount = 0;

cluster::cluster(const std::shared_ptr<DictatorCenter>& c, const std::vector<freq>& sz):
                 _center(c),_size(0), _pointSize(sz), _center_len(c->klen())
{

    for(const freq& i : _pointSize){
        _size += i;
    }
    
    std::vector<size_t> bps;
    
    KmerDecoder decoder(_center_len);

    decoder.bpSequence(c->center(), bps);
    // Initialize the requency for each bp.
    for(size_t i = 0; i < c->klen(); ++i) {
        _bpFrequency.push_back(std::array<int,4>());
        _bpFrequency.back().fill(0);
        _bpFrequency.back()[bps[i]] = static_cast<int>(_size);
        //_bpFrequency.back()[kmersDictionary::getInstance()->asc2dna(sequence[i])] = _size;
    }

    kmers_freq centerbarcode(c->center(),this->_size);
    this->_barcodes.push_back(centerbarcode);
    ClusterCount += 1;
    this->_cid = ClusterCount;
}
cluster::cluster(const std::shared_ptr<DictatorCenter>& center, const std::vector<freq>& time_points,
                 const std::vector<std::array<int, 4>>& frequency_table, int cluster_id):
    _center(center), _pointSize(time_points),_bpFrequency(frequency_table), _cid(cluster_id),_center_len(center->klen())
{
    _size = std::accumulate(_pointSize.begin(), _pointSize.end(), 0);
}
void cluster::updateFrequency(const cluster& cl) {
    const vector<std::array<int,4>>& freqtable = cl.bpFrequency();
    for(size_t i = 0; i < freqtable.size(); i++){
        for(size_t j = 0; j < 4; j++){
            this->_bpFrequency[i][j] += freqtable[i][j];
        }
    }
}
void cluster::updateFrequency(const kmers_freq& k_freq) {
    for(size_t i = 0; i < _center_len; ++i) {
        _bpFrequency[i][0x3&(k_freq._key>>(2*(_center_len - 1 -i)))] += k_freq._freq;
    }
}
void cluster::updateBarcodeList(const std::list<kmers_freq>& barcode_list) {
    assert(_pointSize.size() == 1);
    for (size_t i = 0; i < _bpFrequency.size(); ++i) {
        for (size_t b = 0; b < 4; ++b) {
            _bpFrequency[i][b] = 0;
        }
    }
    _barcodes.assign(barcode_list.begin(), barcode_list.end());
    _size = 0;
    for (const auto& bp : _barcodes) {
        updateFrequency(bp);
        _size += bp._freq;
    }
    _pointSize[0] = _size;
    updateCenter();
}

}	// namespace barcodeSpace
