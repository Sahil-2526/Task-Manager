#include "Scheduler.h"

// operation function
void Scheduler::addTask(TaskManager task){
    tasks.push_back(task);
}
