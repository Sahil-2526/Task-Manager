#ifndef UTILS_GUARD
#define UTILS_GUARD

#include<string>
#include<ctime>

//convert string to time
std::tm stringToTime(std::string);

//convert time to string
std::string timeToString(std::tm);

#endif