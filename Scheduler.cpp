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
    updateScore();
    if(tasks.size()>0){
        cout<<"Task added successfully.\n";
    }
    else{
        cout<<"Failed to add task.\n";
    }
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
    updateScore();
    if(deleted){
        cout<<"Task with ID "<<id<<" deleted successfully.\n";
    }
    else{
        cout<<"Task with ID "<<id<<" not found.\n";
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

void Scheduler::updateScore(){
    double avgEstimated = avgEstimatedTime();
    double avgDeadlineTime = avgTimeToDeadline();
    double avgImportance = avgImportanceLevel();

    for(TaskManager& t : tasks){
        time_t taskEstimatedTime = tmTotime_t(stringToTime(t.getEstimatedTime()));
        time_t taskDeadline = tmTotime_t(stringToTime(t.getDeadline())) - time(0);

        double importanceScore = static_cast<double>(t.getImportanceLvL()) / (1 + avgImportance);
        double estimatedTimeScore = 1.0;
        if(taskEstimatedTime != 0){
            estimatedTimeScore = 1.0 - (double)avgEstimated / taskEstimatedTime;
        }
        double deadlineScore = 1.0;
        if(taskDeadline != 0){
            deadlineScore = 1.0 - (double)avgDeadlineTime / taskDeadline;
        }
        estimatedTimeScore = std::max(0.1, estimatedTimeScore);
        deadlineScore = std::max(0.1, deadlineScore);
        double score = importanceScore * 0.5 +
                       estimatedTimeScore * 0.25 +
                       deadlineScore * 0.25;

        t.setScore(score);
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
double Scheduler::avgEstimatedTime(){
    if(tasks.size()==0){
        return 0;
    }
    double totalSeconds=0;
    for(TaskManager t:tasks){
        totalSeconds+=tmTotime_t(stringToTime(t.getEstimatedTime()));  // time(0) gives current time in seconds since 1 Jan 1970
    }
    return totalSeconds/tasks.size(); 
}

double Scheduler::avgTimeToDeadline(){
    if(tasks.size()==0){
        return 0;
    }
    double totalSeconds=0;
    for(TaskManager t:tasks){
        totalSeconds+=tmTotime_t(stringToTime(t.getDeadline()))-time(0); 
    }
    return totalSeconds/tasks.size(); 
}

double Scheduler::avgImportanceLevel(){
    if(tasks.size()==0){
        return 0;
    }
    double totalImportance=0;
    for(TaskManager t:tasks){
        totalImportance+=t.getImportanceLvL();
    }
    return totalImportance/tasks.size(); 
}