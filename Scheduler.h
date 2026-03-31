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
        void updateScore();

        // sorted display functions
        void showSortedTasksByImportance();
        void showSortedTasksByDeadline();
        void showSortedTasksByEstimatedTime();
        void showTasksByStatus(Status);
        
        // find task
        TaskManager findTaskById(int);

        //average time functions
        double avgEstimatedTime();
        double avgTimeToDeadline();
        double avgImportanceLevel();

        // next task function
        TaskManager nextTask();

        // file handling functions
        void saveToFile(const std::string& filename);
        void loadFromFile(const std::string& filename);
        
};

#endif 