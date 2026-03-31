#ifndef UTILS_GUARD
#define UTILS_GUARD

#include<string>
#include<ctime>

// status enum
enum class Status {Pending, InProgress, Completed,All};

//convert string to time
std::tm stringToTime(std::string);

//convert time to string
std::string timeToString(std::tm);

//compare two time
int compareTime(std::tm,std::tm);

//status to string
std::string statusToString(Status);

//string to status
Status stringToStatus(std::string);

// tm to time_t
time_t tmTotime_t(std::tm);


#endif