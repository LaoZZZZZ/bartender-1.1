//
//  threadwrapper.cpp
//  barcode_project
//
//  Created by luzhao on 1/30/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "threadwrapper.hpp"
#include <unistd.h>

using namespace std;
namespace barcodeSpace {
    ThreadWrapper::ThreadWrapper():_inRunning(0),
    _pThread(),
    _joinable(false)
    {
    }
    /** launch the thread inside*/
    void ThreadWrapper::start(){
        if(_inRunning || _joinable)
            return;
        else{
            this->_inRunning = true;
            this->_joinable = true;
            this->_pThread.reset(new thread(runThread,(void*)this));
	    this->finish();
        }
    }
    /** get the thread ID*/
    thread::id ThreadWrapper::self(){
        return std::this_thread::get_id();
    }
    /** the entry function for the thread*/
    void ThreadWrapper::runThread(void* arg){
        try{
            if(!arg) throw invalid_argument("invalid thread argument");
            ((ThreadWrapper*)arg)->run();
        }catch(const exception& e){
            cerr << e.what() << endl;
            cerr.flush();
            exit(-1);
        }
    }
    void ThreadWrapper::run(){
        throw 1;
    }
    ThreadWrapper::~ThreadWrapper(){
        if (_joinable) {
            _pThread->join();
        }

    }
    void ThreadWrapper::join(){
        if(_joinable){
            _pThread->join();
            _joinable = false;
        }
    }
    void ThreadWrapper::finish(){ this->_inRunning = false;}
}
