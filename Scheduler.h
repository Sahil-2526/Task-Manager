#ifndef SCHEDULER_GUARD
#define SCHEDULER_GUARD

#include "Task.h"
#include <vector> // tasks vector

class Scheduler {
    private:
        std::vector<TaskManager> tasks;
    public:
       // basic functions
        void addTask(TaskManager);
        void showTasks(); 
        void removeTask(int);
        void updateTask(int, TaskManager&);
        void updateScore(TaskManager&);

        // sorted display functions
        void showSortedTasksByImportance();
        void showSortedTasksByDeadline();
        void showSortedTasksByEstimatedTime();
        void showTasksByStatus(Status);
        
        // find task
        TaskManager findTaskById(int);

        //average time functions
        double averageEstimatedTime();
        double averageTimeToDeadline();
        double avgImportanceLevel();
        
};

#endif 