#include "Scheduler.h"

// operation function
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
}

void Scheduler::updateTask(int id, string task, int impLvL, string estimatedTime, string deadline, string status){
    for(int i=0;i<tasks.size();i++){
        if(tasks[i].getId()==id){
            tasks[i]=TaskManager(task,impLvL,estimatedTime,deadline,status);
            break;
        }
    }

}