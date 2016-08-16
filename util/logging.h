/**
* Software License Agreement (BSD License)
*
*           Copyright (C) 2015 by Israel D. Gebru,
*           Perception Team, INRIA-Grenoble, France
*                   All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
*   list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* The views and conclusions contained in the software and documentation are those
* of the authors and should not be interpreted as representing official policies,
* either expressed or implied, of the FreeBSD Project.
*/

#ifndef LOGGING_H
#define LOGGING_H

#include "stdio.h"
#include "stdlib.h"
#include "iostream"
#include "string.h"
#include <sys/time.h>


#define _RED     "\x1b[31m"
#define _GREEN   "\x1b[32m"
#define _YELLOW  "\x1b[33m"
#define _BLUE    "\x1b[34m"
#define _MAGENTA "\x1b[35m"
#define _CYAN    "\x1b[36m"
#define _RESET   "\x1b[0m"
#define _REDB    "\x1b[41m"
#define _GREENB  "\x1b[42m"
#define _RESETB  "\x1b[49m"

enum MessegeType
{
    Critical,
    Error,
    Warning,
    Info,
    Others,
};

typedef unsigned long long timestamp_t;

class Log
{
public:
    Log()
    {
        name_ = "";
        start();
    }
    Log(std::string str)
    {
        name_ = str;
        start();
    }

    ~Log()
    {
        end();
    }

   void messege(std::string msg, bool showtime, MessegeType mtype);
   static void messege(std::string msg, MessegeType mtype);

private:
    timestamp_t start_;
    double duration_;
    std::string name_;
protected:
    static std::string getTimeStamp();
    double getDuration(){return duration_;}
    static timestamp_t gettime()
    {
        struct timeval now;
        gettimeofday(&now, NULL);
        return now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
    }

    inline void start(){ start_ = gettime();}
    inline void end() { duration_ = (gettime() - start_)/1000000.L;}
};



#endif // LOGGING_H

