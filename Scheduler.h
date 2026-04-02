#ifndef SCHEDULER_GUARD
#define SCHEDULER_GUARD

#include "Task.h"
#include <vector>
#include <string>
#include <utility> 

class Scheduler {
    private:
        std::vector<TaskManager> tasks;
        std::vector<TaskManager> recycleBinTasks; 
        std::vector<std::pair<int, int>> todayTasks; 
        
        void reassignIds();

    public:
        void addTask(TaskManager);
        void showTasks(); 
        void removeTask(int); 
        void updateTask(int, TaskManager&);
        void updateScore();

        // GUI Access Getters
        const std::vector<TaskManager>& getTasks() const { return tasks; }
        const std::vector<TaskManager>& getRecycledTasks() const { return recycleBinTasks; }
        const std::vector<std::pair<int, int>>& getTodayTasks() const { return todayTasks; }

        void showSortedTasksByImportance();
        void showSortedTasksByDeadline();
        void showSortedTasksByEstimatedTime();
        void showTasksByStatus(Status);
        
        TaskManager findTaskById(int);

        double avgEstimatedTime();
        double avgTimeToDeadline();
        double avgImportanceLevel();

        TaskManager nextTask();

        void restoreTask(int);
        void permanentlyRemoveTask(int);
        void emptyRecycleBin();

        // Today Tasks & Auto Schedule Methods
        void addToToday(int taskId, int percent);
        void completeTodayTask(int taskId);
        void showTodayTasks();
        void autoScheduleToday();

        // File handling
        void saveToFile(const std::string& filename);
        void loadFromFile(const std::string& filename);
};

#endif