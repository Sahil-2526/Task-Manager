#include "Scheduler.h"
#include "Utils.h"

#include <algorithm> 
#include <iostream> 
#include <string> 
#include <vector> 
#include <stdexcept> 
#include <fstream>
#include <sstream>

using namespace std;

// --- DYNAMIC ID REASSIGNMENT ---
void Scheduler::reassignIds() {
    int currentId = 1;
    // Number active tasks first
    for (auto& t : tasks) {
        t.setId(currentId++);
    }
    // Number recycle bin tasks continuing from active tasks
    for (auto& t : recycleBinTasks) {
        t.setId(currentId++);
    }
    // Sync the global TaskManager counter so new tasks start at the correct number
    TaskManager::setNextId(currentId);
}

// basic functions
void Scheduler::addTask(TaskManager task){
    tasks.push_back(task);
    reassignIds(); // NEW: Renumber tasks
    updateScore();
    if(tasks.size()>0){
        cout<<"Task added successfully.\n";
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
            recycleBinTasks.push_back(tasks[i]); 
            tasks.erase(tasks.begin()+i);
            deleted=true;
            break; 
        }
    }
    
    if(deleted){
        reassignIds(); // NEW: Renumber tasks so gaps are closed
        updateScore();
        cout<<"Task with ID "<<id<<" moved to Recycle Bin.\n";
    }
    else{
        cout<<"Task with ID "<<id<<" not found.\n";
    }
}

void Scheduler::updateTask(int id, TaskManager& task){
    for(int i=0;i<tasks.size();i++){
        if(tasks[i].getId()==id){
            task.setId(id); 
            tasks[i]=task;
            updateScore(); 
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
    sort(sortedTasks.begin(),sortedTasks.end(),[](const TaskManager& t1, const TaskManager& t2){ 
        return t1.getImportanceLvL()>t2.getImportanceLvL();  
    });
    for(TaskManager t:sortedTasks){ 
        t.showTask();
        cout << "---------------------\n";
    }
}

void Scheduler::showSortedTasksByDeadline(){
    vector<TaskManager> sortedTasks=tasks;
    sort(sortedTasks.begin(),sortedTasks.end(),[](const TaskManager& t1, const TaskManager& t2){
        return compareTime(stringToTime(t1.getDeadline()),stringToTime(t2.getDeadline()))==1; 
    });
    for(TaskManager t:sortedTasks){
        t.showTask();
        cout << "---------------------\n";
    }
}

void Scheduler::showSortedTasksByEstimatedTime(){
    vector<TaskManager> sortedTasks=tasks;
    sort(sortedTasks.begin(),sortedTasks.end(),[](const TaskManager& t1, const TaskManager& t2){
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

// average time functions
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

// --- RECYCLE BIN FUNCTIONS ---

void Scheduler::restoreTask(int id){
    for(int i=0;i<recycleBinTasks.size();i++){
        if(recycleBinTasks[i].getId()==id){
            tasks.push_back(recycleBinTasks[i]);
            recycleBinTasks.erase(recycleBinTasks.begin()+i);
            reassignIds(); // NEW: Renumber tasks 
            updateScore();
            cout<<"Task restored successfully.\n";
            return;
        }
    }
}

void Scheduler::permanentlyRemoveTask(int id){
    for(int i=0;i<recycleBinTasks.size();i++){
        if(recycleBinTasks[i].getId()==id){
            recycleBinTasks.erase(recycleBinTasks.begin()+i);
            reassignIds(); // NEW: Renumber tasks
            cout<<"Task permanently deleted.\n";
            return;
        }
    }
}

void Scheduler::emptyRecycleBin(){
    recycleBinTasks.clear();
    reassignIds(); // NEW: Renumber tasks
    cout<<"Recycle Bin emptied.\n";
}

// --- FILE HANDLING ---

void Scheduler::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (auto& t : tasks) {
        file << t.getId() << "|" << t.getTask() << "|" << t.getImportanceLvL() << "|"
             << t.getEstimatedTime() << "|" << t.getDeadline() << "|" << t.getStatus() << "\n";
    }
    file.close();

    std::ofstream recycleFile("recycle_" + filename);
    if (!recycleFile.is_open()) return;
    for (auto& t : recycleBinTasks) {
        recycleFile << t.getId() << "|" << t.getTask() << "|" << t.getImportanceLvL() << "|"
             << t.getEstimatedTime() << "|" << t.getDeadline() << "|" << t.getStatus() << "\n";
    }
    recycleFile.close();
}

void Scheduler::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    tasks.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, taskName, impStr, est, dead, statusStr;

        std::getline(ss, idStr, '|');
        std::getline(ss, taskName, '|');
        std::getline(ss, impStr, '|');
        std::getline(ss, est, '|');
        std::getline(ss, dead, '|');
        std::getline(ss, statusStr, '|');

        TaskManager t(taskName, std::stoi(impStr), est, dead, stringToStatus(statusStr));
        t.setId(std::stoi(idStr)); 
        tasks.push_back(t);
    }

    std::ifstream recycleFile("recycle_" + filename);
    if (!recycleFile.is_open()) return;
    recycleBinTasks.clear();
    while (std::getline(recycleFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, taskName, impStr, est, dead, statusStr;

        std::getline(ss, idStr, '|');
        std::getline(ss, taskName, '|');
        std::getline(ss, impStr, '|');
        std::getline(ss, est, '|');
        std::getline(ss, dead, '|');
        std::getline(ss, statusStr, '|');

        TaskManager t(taskName, std::stoi(impStr), est, dead, stringToStatus(statusStr));
        t.setId(std::stoi(idStr)); 
        recycleBinTasks.push_back(t);
    }
    
    // NEW: Automatically fix and renumber all IDs loaded from files!
    reassignIds(); 
    updateScore();
}