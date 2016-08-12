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

#include "logging.h"

void Log::messege(std::string msg, bool showtime=false, MessageType mtype=Others)
{
    std::string logname = name_.empty()?"":(name_+" ");
    if (showtime==true)
    {
        logname +=(getTimeStamp()+ " \n");
    }
    else
    {
        logname += "\r";
    }

    switch(mtype)
    {
    case Critical:
        printf(_GREENB "%s" _RESETB _RED "%s" _RESET "\n", logname.c_str(), msg.c_str());
        break;
    case Error:
        printf(_GREENB "%s" _RESETB _RED "%s" _RESET "\n", logname.c_str(), msg.c_str());
        break;
    case Warning:
        printf(_GREENB "%s" _RESETB _YELLOW "%s" _RESET "\n",logname.c_str(), msg.c_str());
        break;
    case Info:
        printf(_GREENB "%s" _RESETB _GREEN "%s" _RESET "\n",logname.c_str(), msg.c_str());
        break;
    case Others:
        printf(_GREENB "%s" _RESETB _BLUE "%s" _RESET "\n", logname.c_str(), msg.c_str());
        break;
    default:
        printf("%s\n", msg.c_str());
        break;
    }
}

std::string Log::getTimeStamp()
{
    char            fmt[64], buf[64];
    struct timeval  tv;
    struct tm       *tm;

    gettimeofday(&tv, NULL);
    if((tm = localtime(&tv.tv_sec)) != NULL)
    {
        strftime(fmt, sizeof fmt, "%H:%M:%S.%%06u", tm); //"%Y-%m-%d %H:%M:%S.%%06u %z"
        snprintf(buf, sizeof buf, fmt, tv.tv_usec);
    }
    return std::string(buf);
}
