/*
 * Copyright 2015, luzhao <luzhao1986@gmail.com>
 *
 * This file is part of Barcode project.
 *
 * Barcode project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Barcode project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Barcode project.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

class Timer{
public:
    Timer(ostream& out = cout, const char *msg = "", bool verbose = true):
         _out(out), _msg(msg), _verbose(verbose),_hour(0),_min(0),_second(0) {}
    virtual ~Timer(){
    }
    virtual void elapsed(){throw 1;}
protected:
    bool verbose()const{return _verbose;}

    void write(ostream & out){
        this->elapsed();
        std::ostringstream oss;
        oss << _msg << setfill ('0') << setw (2) << _hour << ":"
                   << setfill ('0') << setw (2) << _min << ":"
                   << setfill ('0') << setw (2) << _second;
        out << oss.str().c_str();
    }

protected:
    ostream&                _out;
    const char *            _msg;
    bool                    _verbose;
protected:
    int                     _hour;
    int                     _min;
    int                     _second;
};
class cpuTimer : public Timer
{
public:
    cpuTimer(ostream& out = cout, const char *msg = "", bool verbose = true):
        Timer(out,msg,verbose),_t(clock()){

    }
    void elapsed(){
        _t = (clock() - _t)/CLOCKS_PER_SEC;
        this->_hour   = static_cast<int>((_t / 60) / 60);
        this->_min = static_cast<int>((_t / 60) % 60);
        this->_second = static_cast<int>((_t % 60));

    }
    ~cpuTimer(){
        if(this->verbose()) write(_out);

    }
private:
    clock_t                     _t;
};

/**
 * Use time() call to keep track of elapsed time between creation and
 * destruction.  If verbose is true, Timer will print a message showing
 * elapsed time to the given output stream upon destruction.
 */
class realTimer : public Timer{
public:
    realTimer(ostream& out = cout, const char *msg = "", bool verbose = true) :
        Timer(out,msg,verbose),_t(time(0)){ }

	/// Optionally print message
    ~realTimer() {
        if(this->verbose()) write(_out);
	}
	
	/// Return elapsed time since Timer object was created
    void elapsed() {
        _t = time(0) - _t;

        this->_hour   = static_cast<int>((_t / 60) / 60);
        this->_min = static_cast<int>((_t / 60) % 60);
        this->_second = static_cast<int>((_t % 60));
    }
private:
    time_t                      _t;

};

static inline void logTime(std::ostream& os, bool nl = true) {
	struct tm *current;
	time_t now;
	time(&now);
	current = localtime(&now);
	std::ostringstream oss;
	oss << setfill('0') << setw(2)
	    << current->tm_hour << ":"
	    << setfill('0') << setw(2)
	    << current->tm_min << ":"
	    << setfill('0') << setw(2)
	    << current->tm_sec;
	if(nl) oss << std::endl;
	os << oss.str().c_str();
}

#endif /*TIMER_H_*/
