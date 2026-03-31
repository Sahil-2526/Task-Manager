#include "Scheduler.h"
#include "Utils.h"

#include <algorithm> // sort
#include <iostream> // cout
#include <string> // string
#include <vector> // vector
#include <stdexcept> // invalid_argument

#include <fstream>
#include <sstream>

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
    if(tasks.empty()){
        cout << "No tasks available.\n";
        return;
    }
    for(TaskManager t:tasks){
        t.showTask(); 
        cout << "---------------------\n";
    }
}

void Scheduler::removeTask(int id){
    bool deleted=false;
    for(int i=0;i<tasks.size();i++){
        if(tasks[i].getId()==id){
            tasks.erase(tasks.begin()+i);
            deleted=true;
            break; // FIXED: break out of loop once found
        }
    }
    // FIXED: Removed the logic that loops through and overwrites task IDs
    
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
            task.setId(id); // FIXED: Keep the original ID intact
            tasks[i]=task;
            updateScore(); // FIXED: recalculate scores after an update
            cout<<"Task updated successfully.\n";
            return;
        }
    }
    cout<<"Task not found.\n";
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
    sort(sortedTasks.begin(),sortedTasks.end(),[](TaskManager t1, TaskManager t2){ 
        return t1.getImportanceLvL()>t2.getImportanceLvL();  
    });
    for(TaskManager t:sortedTasks){ // FIXED: Actually print the sorted results
        t.showTask();
        cout << "---------------------\n";
    }
}

void Scheduler::showSortedTasksByDeadline(){
    vector<TaskManager> sortedTasks=tasks;
    sort(sortedTasks.begin(),sortedTasks.end(),[](TaskManager t1,TaskManager t2){
        return compareTime(stringToTime(t1.getDeadline()),stringToTime(t2.getDeadline()))==1; 
    });
    for(TaskManager t:sortedTasks){
        t.showTask();
        cout << "---------------------\n";
    }
}

void Scheduler::showSortedTasksByEstimatedTime(){
    vector<TaskManager> sortedTasks=tasks;
    sort(sortedTasks.begin(),sortedTasks.end(),[](TaskManager t1,TaskManager t2){
        return compareTime(stringToTime(t1.getEstimatedTime()),stringToTime(t2.getEstimatedTime()))==-1; 
    });
    for(TaskManager t:sortedTasks){
        t.showTask();
        cout << "---------------------\n";
    }
}

void Scheduler::showTasksByStatus(Status status){
    if(status!=Status::Pending && status!=Status::InProgress && status!=Status::Completed && status!=Status::All){
        cout<<"Invalid Status\n";
        return;
    }
    
    // FIXED: Moved Status::All logic entirely OUT of the for loop
    if(status==Status::All){
        cout<<"\n--- Pending Tasks ---\n";
        showTasksByStatus(Status::Pending);
        cout<<"\n--- In Progress Tasks ---\n";
        showTasksByStatus(Status::InProgress);
        cout<<"\n--- Completed Tasks ---\n";
        showTasksByStatus(Status::Completed);
        return; 
    }
    
    for(TaskManager t:tasks){
        if(t.getStatus()==statusToString(status)){
            t.showTask();
            cout << "---------------------\n";
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
    throw invalid_argument("Task with the given ID not found."); 
}

//average time functions
double Scheduler::avgEstimatedTime(){
    if(tasks.size()==0){
        return 0;
    }
    double totalSeconds=0;
    for(TaskManager t:tasks){
        totalSeconds+=tmTotime_t(stringToTime(t.getEstimatedTime()));  
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

// next task function
TaskManager Scheduler::nextTask(){
    if(tasks.size()==0){
        throw invalid_argument("No tasks available."); 
    }
    TaskManager* next=nullptr;
    for(TaskManager& t:tasks){
        if(next==nullptr || t.getScore()>next->getScore()){
            next=&t;
        }
    }
    return *next;
}

// file handling functions

void Scheduler::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (auto& t : tasks) {
        file << t.getId() << "|"
             << t.getTask() << "|"
             << t.getImportanceLvL() << "|"
             << t.getEstimatedTime() << "|"
             << t.getDeadline() << "|"
             << t.getStatus() << "\n";
    }
    file.close();
}

void Scheduler::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    tasks.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string idStr, taskName, impStr, est, dead, statusStr;

        std::getline(ss, idStr, '|');
        std::getline(ss, taskName, '|');
        std::getline(ss, impStr, '|');
        std::getline(ss, est, '|');
        std::getline(ss, dead, '|');
        std::getline(ss, statusStr, '|');

        if(taskName.empty()) continue;

        TaskManager t(taskName, std::stoi(impStr), est, dead, stringToStatus(statusStr));
        t.setId(std::stoi(idStr)); // Restore the original ID
        tasks.push_back(t);
    }
    updateScore();
}