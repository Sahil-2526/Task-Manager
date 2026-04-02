#include "Task.h"
#include <sstream> 
#include <iomanip> 
#include <iostream> 

using namespace std;

//static variable initialization
int TaskManager::nextId=1;

// constructor 
TaskManager::TaskManager(string task,int impLvL,string estimatedTime,string deadline, Status status) {
    this->id=nextId++;
    this->task = task; 
    this->importanceLvL=impLvL;
    this->estimatedTime=stringToTime(estimatedTime);
    this->deadline=stringToTime(deadline);
    this->status=status;
}

// getters function 
int TaskManager::getId() const {
    return id;
}
string TaskManager::getStatus() const {
    return statusToString(status);
}
string TaskManager::getTask() const {
    return task;
}
int TaskManager::getImportanceLvL() const {
    return importanceLvL;
}
string TaskManager::getEstimatedTime() const {
    return timeToString(estimatedTime);
}   
string TaskManager::getDeadline() const {
    return timeToString(deadline);
}
double TaskManager::getScore() const {
    return score;
}
double TaskManager::getProgress() const {
    return progress;
}

// setters function
void TaskManager::setNextId(int val) {
    nextId = val; // NEW: Allows scheduler to sync the counter
}

void TaskManager::setId(int id){
    this->id=id;
}

void TaskManager::setScore(double score){
    this->score=score;
}

void TaskManager::setStatus(Status status){
    this->status=status;
    if(status==Status::Completed){
        importanceLvL=0;
        estimatedTime=stringToTime("1970-01-01 00:00:00"); 
        deadline=stringToTime("1970-01-01 00:00:00"); 
        this->score = 0.0; 
    }
}
void TaskManager::setTask(string task){
    this->task=task;
}
void TaskManager::setImportanceLvL(int importanceLvL){
    this->importanceLvL=importanceLvL;
}
void TaskManager::setEstimatedTime(string estimatedTime){
    this->estimatedTime=stringToTime(estimatedTime);
}
void TaskManager::setDeadline(string deadline){
    this->deadline=stringToTime(deadline); 
}
void TaskManager::setProgess(double progress){
    this->progress=progress;
}


// display function
void TaskManager::showTask() const { 
    cout<<"Task: "<<getTask()<<endl;
    cout<<"ID: "<<getId()<<endl;
    cout<<"Status: "<<getStatus()<<endl;
    cout<<"Importance Level: "<<getImportanceLvL()<<endl;
    cout<<"Estimated Time: "<<getEstimatedTime()<<endl;
    cout<<"Deadline: "<<getDeadline()<<endl;
}