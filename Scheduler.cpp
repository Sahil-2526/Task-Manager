#include "Scheduler.h"
#include "Utils.h"

#include <algorithm> // sort
#include<iostream> // cout
#include<string> // string
#include<vector> // vector

using namespace std;

// basic functions
void Scheduler::addTask(TaskManager task){
    tasks.push_back(task);
}

void Scheduler::showTasks(){
    for(TaskManager t:tasks){
        t.showTask(); // calling showTask() of TaskManager class for each task in vector
    }
}

void Scheduler::removeTask(int id){
    bool deleted=false;
    for(int i=0;i<tasks.size();i++){
        if(tasks[i].getId()==id){
            tasks.erase(tasks.begin()+i);
            deleted=true;
        }
    }

    for(int i=0;i<tasks.size();i++){
        if(tasks[i].getId()!=i+1){
            tasks[i].setId(i+1);
        }    
    }

}

void Scheduler::updateTask(int id, TaskManager& task){
    for(int i=0;i<tasks.size();i++){
        if(tasks[i].getId()==id){
            tasks[i]=task;
            break;
        }
    }
}

// sorted display functions
void Scheduler::showSortedTasksByImportance(){
    vector<TaskManager> sortedTasks=tasks;
    sort(sortedTasks.begin(),sortedTasks.end(),[](TaskManager t1, TaskManager t2){ // custom comparator function to sort by importance level by descending order
        return t1.getImportanceLvL()>t2.getImportanceLvL();  
    });
}

void Scheduler::showSortedTasksByDeadline(){
    vector<TaskManager> sortedTasks=tasks;
    sort(sortedTasks.begin(),sortedTasks.end(),[](TaskManager t1,TaskManager t2){
        return compareTime(stringToTime(t1.getDeadline()),stringToTime(t2.getDeadline()))==1; // custom comparator function to sort by deadline by descending order
    });
    for(TaskManager t:sortedTasks){
        t.showTask();
    }
}

void Scheduler::showSortedTasksByEstimatedTime(){
    vector<TaskManager> sortedTasks=tasks;
    sort(sortedTasks.begin(),sortedTasks.end(),[](TaskManager t1,TaskManager t2){
        return compareTime(stringToTime(t1.getEstimatedTime()),stringToTime(t2.getEstimatedTime()))==-1; // custom comparator function to sort by estimated time by ascending order
    });
    for(TaskManager t:sortedTasks){
        t.showTask();
    }
}

void Scheduler::showTasksByStatus(Status status){
    if(status!=Status::Pending && status!=Status::InProgress && status!=Status::Completed && status!=Status::All){
        cout<<"Invalid Status\n";
        return;
    }
    for(TaskManager t:tasks){
        if(status==Status::All){
            cout<<"All Tasks:\n";
    cout<<"Pending Tasks:\n";
    showTasksByStatus(Status::Pending);
    cout<<"In Progress Tasks:\n";
    showTasksByStatus(Status::InProgress);
    cout<<"Completed Tasks:\n";
    showTasksByStatus(Status::Completed);
        }
        else if(t.getStatus()==statusToString(status)){
            t.showTask();
        }
    }
}

// find task
TaskManager Scheduler::findTaskById(int id){
    for(TaskManager t:tasks){
        if(t.getId()==id){
            return t;
        }
    }
    throw invalid_argument("Task with the given ID not found."); //  throw custom error if task with the given ID is not found
}

//average time functions
double Scheduler::averageEstimatedTime(){
    if(tasks.size()==0){
        return 0;
    }
    double totalSeconds=0;
    for(TaskManager t:tasks){
        totalSeconds+=tmTotime_t(stringToTime(t.getEstimatedTime()));  // time(0) gives current time in seconds since 1 Jan 1970
    }
    return totalSeconds/tasks.size(); 
}

double Scheduler::averageTimeToDeadline(){
    if(tasks.size()==0){
        return 0;
    }
    double totalSeconds=0;
    for(TaskManager t:tasks){
        totalSeconds+=tmTotime_t(stringToTime(t.getDeadline()))-time(0); 
    }
    return totalSeconds/tasks.size(); 
}