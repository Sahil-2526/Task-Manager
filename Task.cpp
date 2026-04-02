#include "Task.h"
#include <sstream> 
#include <iomanip> 
#include <iostream> 

using namespace std;

int TaskManager::nextId=1;

TaskManager::TaskManager(string task,int impLvL,string estimatedTime,string deadline, Status status) {
    this->id=nextId++;
    this->task = task; 
    this->importanceLvL=impLvL;
    this->estimatedTime=stringToTime(estimatedTime);
    this->deadline=stringToTime(deadline);
    this->status=status;
    this->progress = 0.0;
    this->hasSchedule = false; 
}

int TaskManager::getId() const { return id; }
string TaskManager::getStatus() const { return statusToString(status); }
string TaskManager::getTask() const { return task; }
int TaskManager::getImportanceLvL() const { return importanceLvL; }
string TaskManager::getEstimatedTime() const { return timeToString(estimatedTime); }   
string TaskManager::getDeadline() const { return timeToString(deadline); }
double TaskManager::getScore() const { return score; }
double TaskManager::getProgress() const { return progress; }

bool TaskManager::getHasSchedule() const { return hasSchedule; }
string TaskManager::getScheduledDate() const { return timeToString(scheduledDate); }
std::tm TaskManager::getScheduledDateTm() const { return scheduledDate; }

void TaskManager::setNextId(int val) { nextId = val; }
void TaskManager::setId(int id){ this->id=id; }
void TaskManager::setScore(double score){ this->score=score; }

void TaskManager::setStatus(Status status){
    this->status=status;
    if(status==Status::Completed){
        importanceLvL=0;
        estimatedTime=stringToTime("1970-01-01 00:00:00"); 
        deadline=stringToTime("1970-01-01 00:00:00"); 
        this->score = 0.0; 
        this->progress = 100.0;
    }
}
void TaskManager::setTask(string task){ this->task=task; }
void TaskManager::setImportanceLvL(int importanceLvL){ this->importanceLvL=importanceLvL; }
void TaskManager::setEstimatedTime(string estimatedTime){ this->estimatedTime=stringToTime(estimatedTime); }
void TaskManager::setDeadline(string deadline){ this->deadline=stringToTime(deadline); }

void TaskManager::setProgess(double progress){
    if(progress<0.0) progress=0.0;
    else if(progress>100.0) progress=100.0;

    this->progress=progress;

    if(progress==100.0){
        setStatus(Status::Completed);
    }
    else if(progress>0.0 && progress<100.0){
        setStatus(Status::InProgress);
    }
    else if(progress==0.0){
        setStatus(Status::Pending);
    }
}

void TaskManager::setScheduledDate(std::string date) {
    this->scheduledDate = stringToTime(date);
    this->hasSchedule = true;
}
void TaskManager::clearScheduledDate() {
    this->hasSchedule = false;
}

void TaskManager::showTask() const { 
    cout<<"Task: "<<getTask()<<endl;
    cout<<"ID: "<<getId()<<endl;
    cout<<"Status: "<<getStatus()<<endl;
    cout<<"Progress: "<<getProgress()<<"%"<<endl;
    cout<<"Importance Level: "<<getImportanceLvL()<<endl;
    cout<<"Estimated Time: "<<getEstimatedTime()<<endl;
    cout<<"Deadline: "<<getDeadline()<<endl;
}