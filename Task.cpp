#include "Task.h"

#include <string>
#include<ctime> // tm
#include<sstream> // istringstream
#include<iomanip> // get_time
#include<iostream>

using namespace std;

tm TaskManager::stringToTime(string s){
    tm time={};
    istringstream ss(s); // convert string to stream
    // helps to extract values sequentially
    ss>>get_time(&time,"%Y-%m-%d %H:%M:%S"); // automatically separate year,month, ...
    // get_time() parse ss according to string pattern given and store values in tm
    if (ss.fail()) { // checks if ss stream parsed successfully  
        cout << "Invalid Format\n";
    }
    return time;
}

TaskManager::TaskManager(string task,int impLvL,string estimatedTime,string deadline, string status) {
    this->id=nextId++;
    this->importanceLvL=impLvL;
    this->estimatedTime=stringToTime(estimatedTime);
    this->deadline=stringToTime(deadline);
    this->status=status;
}