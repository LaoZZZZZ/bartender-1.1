#ifndef CLUSTEROUTPUT_H
#define CLUSTEROUTPUT_H
#include "suffix_commons.h"
#include "cluster.h"
namespace prefixMatching{
class clusterOutput
{
public:
    clusterOutput(std::string outfile,const std::vector<std::string>& ,bool head = true);
    bool write(const list<std::shared_ptr<cluster>>& );
    ~clusterOutput(){
        this->_out.close();
    }
private:
    clusterOutput(const clusterOutput&);
    clusterOutput& operator=(const clusterOutput&);
    void init();
private:
    std::string                     _filename;
    std::fstream                    _out;
    std::vector<std::string>        _columns;
    bool                            _head;
};
}
#endif // CLUSTEROUTPUT_H
