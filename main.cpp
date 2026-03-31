#include "Scheduler.h"
#include <iostream>
using namespace std;

int main(){
      Scheduler scheduler;
    do{
        cout<<"Welcome to Task Manager!\n";
        cout<<"1. Add Task\n";
        cout<<"2. Show Tasks\n";
        cout<<"3. Remove Task\n";
        cout<<"4. Update Task\n";
        cout<<"5. Show Sorted Tasks by Importance\n";
        cout<<"6. Show Sorted Tasks by Deadline\n";
        cout<<"7. Show Sorted Tasks by Estimated Time\n";
        cout<<"8. Show Tasks by Status\n";
        cout<<"9. Next Task To Do\n";
        cout<<"10. Exit\n";

        int choice;
        cin>>choice;

        switch(choice){
            case 1:
                {
                    string task,estimatedTime,deadline;
                    int importanceLvL;
                    Status status;

                    cout<<"Enter task description: ";
                    cin.ignore();
                    getline(cin,task);
                    cout<<"Enter importance level (1-5): ";
                    cin>>importanceLvL;
                    cout<<"Enter estimated time (YYYY-MM-DD HH:MM:SS): ";
                    cin.ignore();
                    getline(cin,estimatedTime);
                    cout<<"Enter deadline (YYYY-MM-DD HH:MM:SS): ";
                    getline(cin,deadline);
                    cout<<"Enter status (Pending/InProgress/Completed): ";
                    string statusStr;
                    getline(cin,statusStr);
                    status=stringToStatus(statusStr);

                    TaskManager newTask(task,importanceLvL,estimatedTime,deadline,status);
                    scheduler.addTask(newTask);
                }
                break;
            case 2:
                scheduler.showTasks();
                break;
            case 3:
                {
                    int id;
                    cout<<"Enter task ID to remove: ";
                    cin>>id;
                    scheduler.removeTask(id);
                }
                break;
            case 4:
                {
                    int id;
                    string task,estimatedTime,deadline;
                    int importanceLvL;
                    Status status;

                    cout<<"Enter task ID to update: ";
                    cin>>id;
                    
                    cout<<"Enter new task description: ";
                    cin.ignore();
                    getline(cin,task);
                    cout<<"Enter new importance level : ";
                    cin>>importanceLvL;
                    cout<<"Enter new estimated time (YYYY-MM-DD HH:MM:SS): ";
                    cin.ignore();
                    getline(cin,estimatedTime);
                    cout<<"Enter new deadline (YYYY-MM-DD HH:MM:SS): ";
                    getline(cin,deadline);
                    cout<<"Enter new status (Pending/InProgress/Completed): ";
                    string statusStr;
                    getline(cin,statusStr);
                    status=stringToStatus(statusStr);

                    TaskManager updatedTask(task,importanceLvL,estimatedTime,deadline,status);
                    scheduler.updateTask(id, updatedTask);
                }
                break;
            case 5:
                scheduler.showSortedTasksByImportance();
                break;
            case 6:
                scheduler.showSortedTasksByDeadline();
                break;
            case 7:
                scheduler.showSortedTasksByEstimatedTime();
                break;
            case 8:
                {
                    Status status;
                    string statusStr;
                    cout<<"Enter status (Pending/InProgress/Completed/All): ";
                    cin.ignore(); // FIXED: Clear newline buffer
                    getline(cin,statusStr);
                    status=stringToStatus(statusStr);
                    scheduler.showTasksByStatus(status);
                }
                break;
            case 9:
                {
                    try {
                        TaskManager next = scheduler.nextTask(); // FIXED: Catch and display the task
                        cout << "\n--- Next Task To Do ---\n";
                        next.showTask();
                    } catch (const invalid_argument& e) {
                        cout << e.what() << "\n";
                    }
                }
                break;
            case 10:
                cout<<"Exiting...\n";
                return 0;
            default:
                cout<<"Invalid choice. Please try again.\n";
        }
    }while(true);
    return 0;
}