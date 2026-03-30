#include "Scheduler.h"
#include "Utils.h"

#include <algorithm> // sort

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

void Scheduler::updateTask(int id, string task, int impLvL, string estimatedTime, string deadline, string status){
    for(int i=0;i<tasks.size();i++){
        if(tasks[i].getId()==id){
            tasks[i]=TaskManager(task,impLvL,estimatedTime,deadline,status);
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
        return compareTime(stringToTime(t1.getDeadline()),stringToTime(t2.getDeadline()))==-1; // custom comparator function to sort by deadline by ascending order
    });
    for(TaskManager t:sortedTasks){
        t.showTask();
    }
}