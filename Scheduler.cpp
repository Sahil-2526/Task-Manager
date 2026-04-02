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

void Scheduler::reassignIds() {
    int currentId = 1;
    for (auto& t : tasks) { t.setId(currentId++); }
    for (auto& t : recycleBinTasks) { t.setId(currentId++); }
    TaskManager::setNextId(currentId);
}

void Scheduler::addTask(TaskManager task){
    tasks.push_back(task);
    reassignIds(); 
    updateScore();
}

void Scheduler::showTasks(){
    if(tasks.empty()){ cout << "No tasks available.\n"; return; }
    for(TaskManager t:tasks){ t.showTask(); cout << "---------------------\n"; }
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
        reassignIds(); 
        updateScore();
    }
}

void Scheduler::updateTask(int id, TaskManager& task){
    for(int i=0;i<tasks.size();i++){
        if(tasks[i].getId()==id){
            task.setId(id); 
            tasks[i]=task;
            updateScore(); 
            return;
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
        if(taskEstimatedTime != 0) estimatedTimeScore = 1.0 - (double)avgEstimated / taskEstimatedTime;
        
        double deadlineScore = 1.0;
        if(taskDeadline != 0) deadlineScore = 1.0 - (double)avgDeadlineTime / taskDeadline;
        
        estimatedTimeScore = std::max(0.1, estimatedTimeScore);
        deadlineScore = std::max(0.1, deadlineScore);
        double score = importanceScore * 0.5 + estimatedTimeScore * 0.25 + deadlineScore * 0.25;

        t.setScore(score);
    }
}

void Scheduler::showSortedTasksByImportance(){
    vector<TaskManager> sortedTasks=tasks;
    sort(sortedTasks.begin(),sortedTasks.end(),[](const TaskManager& t1, const TaskManager& t2){ return t1.getImportanceLvL()>t2.getImportanceLvL(); });
    for(TaskManager t:sortedTasks){ t.showTask(); cout << "---------------------\n"; }
}

void Scheduler::showSortedTasksByDeadline(){
    vector<TaskManager> sortedTasks=tasks;
    sort(sortedTasks.begin(),sortedTasks.end(),[](const TaskManager& t1, const TaskManager& t2){ return compareTime(stringToTime(t1.getDeadline()),stringToTime(t2.getDeadline()))==1; });
    for(TaskManager t:sortedTasks){ t.showTask(); cout << "---------------------\n"; }
}

void Scheduler::showSortedTasksByEstimatedTime(){
    vector<TaskManager> sortedTasks=tasks;
    sort(sortedTasks.begin(),sortedTasks.end(),[](const TaskManager& t1, const TaskManager& t2){ return compareTime(stringToTime(t1.getEstimatedTime()),stringToTime(t2.getEstimatedTime()))==-1; });
    for(TaskManager t:sortedTasks){ t.showTask(); cout << "---------------------\n"; }
}

void Scheduler::showTasksByStatus(Status status){
    if(status!=Status::Pending && status!=Status::InProgress && status!=Status::Completed && status!=Status::All) return;
    
    if(status==Status::All){
        showTasksByStatus(Status::Pending);
        showTasksByStatus(Status::InProgress);
        showTasksByStatus(Status::Completed);
        return; 
    }
    
    for(TaskManager t:tasks){
        if(t.getStatus()==statusToString(status)){
            t.showTask(); cout << "---------------------\n";
        }
    }
}

TaskManager Scheduler::findTaskById(int id){
    for(TaskManager t:tasks){ if(t.getId()==id) return t; }
    throw invalid_argument("Task not found."); 
}

double Scheduler::avgEstimatedTime(){
    if(tasks.size()==0) return 0;
    double totalSeconds=0;
    for(TaskManager t:tasks) totalSeconds+=tmTotime_t(stringToTime(t.getEstimatedTime()));  
    return totalSeconds/tasks.size(); 
}

double Scheduler::avgTimeToDeadline(){
    if(tasks.size()==0) return 0;
    double totalSeconds=0;
    for(TaskManager t:tasks) totalSeconds+=tmTotime_t(stringToTime(t.getDeadline()))-time(0); 
    return totalSeconds/tasks.size(); 
}

double Scheduler::avgImportanceLevel(){
    if(tasks.size()==0) return 0;
    double totalImportance=0;
    for(TaskManager t:tasks) totalImportance+=t.getImportanceLvL();
    return totalImportance/tasks.size(); 
}

TaskManager Scheduler::nextTask(){
    if(tasks.size()==0) throw invalid_argument("No tasks."); 
    TaskManager* next=nullptr;
    for(TaskManager& t:tasks){ if(next==nullptr || t.getScore()>next->getScore()) next=&t; }
    return *next;
}

void Scheduler::restoreTask(int id){
    for(int i=0;i<recycleBinTasks.size();i++){
        if(recycleBinTasks[i].getId()==id){
            tasks.push_back(recycleBinTasks[i]);
            recycleBinTasks.erase(recycleBinTasks.begin()+i);
            reassignIds(); 
            updateScore();
            return;
        }
    }
}

void Scheduler::permanentlyRemoveTask(int id){
    for(int i=0;i<recycleBinTasks.size();i++){
        if(recycleBinTasks[i].getId()==id){
            recycleBinTasks.erase(recycleBinTasks.begin()+i);
            reassignIds(); 
            return;
        }
    }
}

void Scheduler::emptyRecycleBin(){
    recycleBinTasks.clear();
    reassignIds(); 
}

// --- TODAY TASKS & AUTO SCHEDULING SYSTEM ---

void Scheduler::addToToday(int taskId, int percent) {
    if (percent < 1 || percent > 100) return;

    bool exists = false;
    for (const auto& t : tasks) {
        if (t.getId() == taskId) { exists = true; break; }
    }
    if (!exists) return;

    for (const auto& pair : todayTasks) {
        if (pair.first == taskId) return;
    }

    todayTasks.push_back({taskId, percent});
}

void Scheduler::completeTodayTask(int taskId) {
    for (auto it = todayTasks.begin(); it != todayTasks.end(); ++it) {
        if (it->first == taskId) {
            int targetPercent = it->second;

            for (auto& t : tasks) {
                if (t.getId() == taskId) {
                    double newProgress = t.getProgress() + targetPercent;
                    if (newProgress > 100.0) newProgress = 100.0;
                    t.setProgess(newProgress); 
                    break;
                }
            }
            todayTasks.erase(it);
            return;
        }
    }
}

void Scheduler::showTodayTasks() {
    if (todayTasks.empty()) { cout << "No tasks scheduled for today.\n"; return; }
    cout << "\n=== TODAY'S ACTION PLAN ===\n";
    for (const auto& pair : todayTasks) {
        try {
            TaskManager t = findTaskById(pair.first);
            cout << "[ID: " << t.getId() << "] " << t.getTask() << "\n"
                 << "    Current Progress: " << t.getProgress() << "%\n"
                 << "    Goal for Today: +" << pair.second << "%\n";
        } catch (...) {}
    }
}

void Scheduler::autoScheduleToday() {
    time_t now = time(0);
    std::tm today = *localtime(&now);
    
    for (const auto& t : tasks) {
        if (t.getHasSchedule() && isSameDay(t.getScheduledDateTm(), today)) {
            bool alreadyIn = false;
            for (const auto& pair : todayTasks) {
                if (pair.first == t.getId()) { alreadyIn = true; break; }
            }
            if (!alreadyIn) {
                todayTasks.push_back({t.getId(), 20});
            }
        }
    }
}

// --- FILE HANDLING ---
void Scheduler::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (auto& t : tasks) {
        file << t.getId() << "|" << t.getTask() << "|" << t.getImportanceLvL() << "|"
             << t.getEstimatedTime() << "|" << t.getDeadline() << "|" << t.getStatus() << "|" 
             << t.getProgress() << "|" << t.getHasSchedule() << "|" << t.getScheduledDate() << "\n";
    }
    file.close();

    std::ofstream recycleFile("recycle_" + filename);
    if (!recycleFile.is_open()) return;
    for (auto& t : recycleBinTasks) {
        recycleFile << t.getId() << "|" << t.getTask() << "|" << t.getImportanceLvL() << "|"
             << t.getEstimatedTime() << "|" << t.getDeadline() << "|" << t.getStatus() << "|" 
             << t.getProgress() << "|" << t.getHasSchedule() << "|" << t.getScheduledDate() << "\n";
    }
    recycleFile.close();
}

void Scheduler::loadFromFile(const std::string& filename) {
    auto parseLine = [](const std::string& line, std::vector<TaskManager>& targetList) {
        if (line.empty()) return;
        std::stringstream ss(line);
        std::string idStr, taskName, impStr, est, dead, statusStr, progStr, hasSchedStr, schedDateStr;

        std::getline(ss, idStr, '|');
        std::getline(ss, taskName, '|');
        std::getline(ss, impStr, '|');
        std::getline(ss, est, '|');
        std::getline(ss, dead, '|');
        std::getline(ss, statusStr, '|');
        std::getline(ss, progStr, '|'); 
        std::getline(ss, hasSchedStr, '|');
        std::getline(ss, schedDateStr, '|');

        TaskManager t(taskName, std::stoi(impStr), est, dead, stringToStatus(statusStr));
        t.setId(std::stoi(idStr)); 
        if(!progStr.empty()) t.setProgess(std::stod(progStr)); 
        if(hasSchedStr == "1") t.setScheduledDate(schedDateStr);

        targetList.push_back(t);
    };

    std::ifstream file(filename);
    if (file.is_open()) {
        tasks.clear();
        std::string line;
        while (std::getline(file, line)) parseLine(line, tasks);
        file.close();
    }

    std::ifstream recycleFile("recycle_" + filename);
    if (recycleFile.is_open()) {
        recycleBinTasks.clear();
        std::string line;
        while (std::getline(recycleFile, line)) parseLine(line, recycleBinTasks);
        recycleFile.close();
    }
    
    reassignIds(); 
    updateScore();
}