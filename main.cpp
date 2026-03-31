#include "Scheduler.h"
#include <iostream>
using namespace std;

int main(){
    Scheduler scheduler;

    // Adding tasks
    scheduler.addTask(TaskManager("Complete project report", 5, "2024-07-01 17:00:00", "2024-07-10 17:00:00", Status::Pending));
    scheduler.addTask(TaskManager("Prepare presentation", 4, "2024-07-05 10:00:00", "2024-07-15 10:00:00", Status::InProgress));
    scheduler.addTask(TaskManager("Team meeting", 3, "2024-07-03 14:00:00", "2024-07-03 15:00:00", Status::Completed));

    // Displaying tasks
    cout << "All Tasks:\n";
    scheduler.showTasks();

    // Displaying sorted tasks by importance
    cout << "\nTasks Sorted by Importance:\n";
    scheduler.showSortedTasksByImportance();

    // Displaying sorted tasks by deadline
    cout << "\nTasks Sorted by Deadline:\n";
    scheduler.showSortedTasksByDeadline();

    // Displaying sorted tasks by estimated time
    cout << "\nTasks Sorted by Estimated Time:\n";
    scheduler.showSortedTasksByEstimatedTime();

    // Displaying tasks by status
    cout << "\nPending Tasks:\n";
    scheduler.showTasksByStatus(Status::Pending);

    cout << "\nIn Progress Tasks:\n";
    scheduler.showTasksByStatus(Status::InProgress);

    cout << "\nCompleted Tasks:\n";
    scheduler.showTasksByStatus(Status::Completed);
}