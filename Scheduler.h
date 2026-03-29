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
};

#endif