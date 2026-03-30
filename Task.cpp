#include "Task.h"
#include "Utils.h"

#include<sstream> // istringstream
#include<iomanip> // get_time
#include<iostream> //cout 

using namespace std;

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

//setters function
void TaskManager::setId(int id){
    this->id=id;
}


void TaskManager::showTask(){
    cout<<"Task: "<<getTask()<<endl;
    cout<<"ID: "<<getId()<<endl;
    cout<<"Status: "<<getStatus()<<endl;
    cout<<"Importance Level: "<<getImportanceLvL()<<endl;
    cout<<"Estimated Time: "<<getEstimatedTime()<<endl;
    cout<<"Deadline: "<<getDeadline()<<endl;
}


