#ifndef SCHEDULER_GUARD
#define SCHEDULER_GUARD

#include "Task.h"
#include <vector>

using namespace std;

class Scheduler {
    private:
        std::vector<TaskManager> tasks;
    public:
        void addTask(TaskManager);
        void showTasks(); 
        void removeTask(int);
        void updateTask(int, string, int, string, string, string);
};

#endif