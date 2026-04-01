#ifndef SCHEDULER_GUARD
#define SCHEDULER_GUARD

#include "Task.h"
#include <vector>
#include <string>

class Scheduler {
    private:
        std::vector<TaskManager> tasks;
        std::vector<TaskManager> recycleBinTasks; 
        
        void reassignIds(); // NEW: Helper to dynamically renumber all tasks

    public:
        void addTask(TaskManager);
        void showTasks(); 
        void removeTask(int); 
        void updateTask(int, TaskManager&);
        void updateScore();

        // Getter for GUI access
        const std::vector<TaskManager>& getTasks() const { return tasks; }
        const std::vector<TaskManager>& getRecycledTasks() const { return recycleBinTasks; }

        void showSortedTasksByImportance();
        void showSortedTasksByDeadline();
        void showSortedTasksByEstimatedTime();
        void showTasksByStatus(Status);
        
        TaskManager findTaskById(int);

        double avgEstimatedTime();
        double avgTimeToDeadline();
        double avgImportanceLevel();

        TaskManager nextTask();

        // Recycle Bin features
        void restoreTask(int);
        void permanentlyRemoveTask(int);
        void emptyRecycleBin();

        // File handling
        void saveToFile(const std::string& filename);
        void loadFromFile(const std::string& filename);
};

#endif