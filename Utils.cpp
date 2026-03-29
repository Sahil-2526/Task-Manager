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