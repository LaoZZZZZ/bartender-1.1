//
//  threadwrapper.hpp
//  barcode_project
//
//  Created by luzhao on 1/30/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef threadwrapper_hpp
#define threadwrapper_hpp


#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>

namespace barcodeSpace {

/**
 * a thread wrapper for easy use of thread
 * it could be subclassed for different task
 */
class ThreadWrapper
{
public:
    ThreadWrapper();
    template <typename T>
    ThreadWrapper(T&){}
    void start();
    void join();
    void finish();
    void yield(){std::this_thread::yield();}
    bool joinable(){return this->_joinable;}
    void enableJoin(){this->_joinable= true;}
    static void runThread(void *p);
    std::thread::id self();
    bool  isfinished() {return !_inRunning;}
    virtual~ThreadWrapper();
private:
    virtual void run();
protected:
    bool                                    _inRunning;
    std::unique_ptr<std::thread>            _pThread;
    bool                                    _joinable;
};
}   // namespace barcodeSpace
#endif /* threadwrapper_hpp */
