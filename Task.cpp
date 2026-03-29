#include "Task.h"

#include <string>
#include<ctime> // tm
#include<sstream> // istringstream
#include<iomanip> // get_time
#include<iostream>

using namespace std;

// string to time  and vice versa converting function
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
string timeToString(tm time){
    ostringstream s;
    s<<put_time(&time,"%Y-%m-%d %H:%M:%S"); // convert tm to string according to pattern
    return s.str();
}

//static variable initialization
int TaskManager::nextId=1;

// constructor 
TaskManager::TaskManager(string task,int impLvL,string estimatedTime,string deadline, string status) {
    this->id=nextId++;
    this->importanceLvL=impLvL;
    this->estimatedTime=stringToTime(estimatedTime);
    this->deadline=stringToTime(deadline);
    this->status=status;
}


//getters function
int TaskManager::getId(){
    return id;
}
string TaskManager::getStatus(){
    return status;
}
string TaskManager::getTask(){
    return task;
}
int TaskManager::getImportanceLvL(){
    return importanceLvL;
}
string TaskManager::getEstimatedTime(){
    return timeToString(estimatedTime);
}   
string TaskManager::getDeadline(){
    return timeToString(deadline);
}


void TaskManager::showTask(){
    cout<<"Task: "<<getTask()<<endl;
    cout<<"ID: "<<getId()<<endl;
    cout<<"Status: "<<getStatus()<<endl;
    cout<<"Importance Level: "<<getImportanceLvL()<<endl;
    cout<<"Estimated Time: "<<getEstimatedTime()<<endl;
    cout<<"Deadline: "<<getDeadline()<<endl;
}


