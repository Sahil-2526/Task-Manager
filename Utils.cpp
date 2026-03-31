#include "Utils.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <ctime>

// convert string to time
tm stringToTime(std::string s){
    tm time={};
    std::istringstream ss(s); // convert string to stream
    // helps to extract values sequentially
    ss>>std::get_time(&time,"%Y-%m-%d %H:%M:%S"); // automatically separate year,month, ...
    // get_time() parse ss according to string pattern given and store values in tm
    if (ss.fail()) { // checks if ss stream parsed successfully  
        std::cout << "Invalid Format\n";
    }
    return time;
}

// convert time to string
std::string timeToString(tm time){
    std::ostringstream s;
    s<<std::put_time(&time,"%Y-%m-%d %H:%M:%S"); // convert tm to string according to pattern
    return s.str();
}

// compare two time 
int compareTime(tm t1,tm t2){
    if(t1.tm_year>t2.tm_year) return 1;
    if(t1.tm_mon>t2.tm_mon) return 1;
    if(t1.tm_mday>t2.tm_mday) return 1;
    if(t1.tm_hour>t2.tm_hour) return 1;
    if(t1.tm_min>t2.tm_min) return 1;
    if(t1.tm_sec>t2.tm_sec) return 1;

    if(t1.tm_year<t2.tm_year) return -1;
    if(t1.tm_mon<t2.tm_mon) return -1;
    if(t1.tm_mday<t2.tm_mday) return -1;
    if(t1.tm_hour<t2.tm_hour) return -1;
    if(t1.tm_min<t2.tm_min) return -1;
    if(t1.tm_sec<t2.tm_sec) return -1;

    return 0;
}

// status to string
std::string statusToString(Status s){
    if(s==Status::Pending){
        return "Pending";
    }
    else if(s==Status::InProgress){
        return "In Progress";
    }
    else{
        return "Completed";
    }
}

// string to status
Status stringToStatus(std::string s){
    if(s=="Pending"){
        return Status::Pending;
    }
    else if(s=="In Progress"){
        return Status::InProgress;
    }
    else{
        return Status::Completed;
    }
}

// tm to time_t
time_t tmTotime_t(tm time){
    return mktime(&time); }  // converts tm to time_t // no. of seconds since 1 Jan 1970