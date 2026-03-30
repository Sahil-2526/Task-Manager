#ifndef SCHEDULER_GUARD
#define SCHEDULER_GUARD

#include "Task.h"
#include <vector> // tasks vector
#include <algorithm> // sort

using namespace std;

class Scheduler {
    private:
        std::vector<TaskManager> tasks;
    public:
       // basic functions
        void addTask(TaskManager);
        void showTasks(); 
        void removeTask(int);
        void updateTask(int, string, int, string, string, string);

        // sorted display functions
        void showTasksByImportance();
    
};

#endif