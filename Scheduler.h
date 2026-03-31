#ifndef SCHEDULER_GUARD
#define SCHEDULER_GUARD

#include "Task.h"
#include <vector> // tasks vector


using namespace std;

class Scheduler {
    private:
        std::vector<TaskManager> tasks;
    public:
       // basic functions
        void addTask(TaskManager);
        void showTasks(); 
        void removeTask(int);
        void updateTask(int, TaskManager&);

        // sorted display functions
        void showSortedTasksByImportance();
        void showSortedTasksByDeadline();
        void showSortedTasksByEstimatedTime();
        void showTasksByStatus(Status);
        
        // arranged display functions   
        void showArrangedTasksByStatus(Status);
};

#endif