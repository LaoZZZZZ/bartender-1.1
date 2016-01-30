#include "clusterinput.h"
namespace prefixMatching{
clusterInput::clusterInput(const std::string& filename):_filename(filename),
                           _bins({}),fb_(),_head({})
{
}
void clusterInput::load(){
    FILE* fb = fopen(this->_filename.c_str(),"r");
    this->fb_.reset(new FileBuf(fb));

    string line;
    list<string> fields;
    list<freq>    columns;

    if(!this->fb_->eof()){
        this->fb_->getline(line);
        strSplit(line,fields);
        // if the file has head, keep the head for later use
        if(fields.size() && fields.front() == "center"){
            _head.assign(fields.begin(),fields.end());
        }
        size_t columnSize(fields.size());
        BTDnaString center;
        while(!this->fb_->eof()){
            // 1.get one cluster
            this->fb_->getline(line);
            // 2. split the line into fileds
            strSplit(line,fields);
            // 3. check if the fileds size are consistent to previous lines
            if(fields.size() != columnSize)
                throw runtime_error(this->_filename + "does not have valid file format!\n");
            //4. the first column should be the center(barcode)
            center.installChars(fields.front().c_str(),fields.front().length());
            kmer c;
            kmers_BitwiseTransform::getInstance()->seq_2_bitwise(center,c,(size_t)(center.length()));
            fields.pop_front();
            // 5. get the frequency for each time point or experiment
            freq f(0);
            for(const string& sz:fields){
                // transfer the string to integer
                int csz = atoi(sz.c_str());
                //
                columns.push_back(csz);
                f += csz;
            }
            Center* tmp = new dictatorCenter(c,center.length());
            std::shared_ptr<Center> pcenter(tmp);
            cluster *cl = new dictatorCluster(pcenter,columns);
            std::shared_ptr<cluster> pcl(cl);
            if(this->_bins.count(center.length())){
                this->_bins[center.length()].push_back(pcl);
            }else{
                std::list<std::shared_ptr<cluster> > lst({pcl});
                this->_bins[center.length()] = lst;
            }
            line.clear();
            center.clear();
            columns.clear();
        }
    }

}
}// prefixMatching
