#include "testSimulation.h"

namespace prefixMatching{
void loadData(std::string file,kmers_sparseHash<kmer,freq>& table){
    std::fstream handler(file.c_str(),fstream::in);
    if(handler.is_open()){
        table.clear();
        while(!handler.eof()){
            kmer k;
            freq f;
            handler >> k >>f;
            //cout<<k<<'\t'<<f<<endl;

            table[k] = f;
        }
    }else
        throw runtime_error("Invalid file to read!\n");
    handler.close();
}
void outputCluster(std::string outfile,const std::list<std::shared_ptr<cluster>>& c){
    fstream outhandler(outfile.c_str(),fstream::out);
    for(auto iter = c.begin(); iter != c.end(); iter++){
        outhandler<<(*iter)->center()->center()<<','<<(*iter)->size()<<endl;
    }
    outhandler.close();
}
}
