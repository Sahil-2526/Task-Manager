#ifndef TASK_GUARD  
#define TASK_GUARD 

#include <string>
#include <ctime> 
#include "Utils.h"

class TaskManager {
private:
    static int nextId;
    int id;
    std::string task;
    int importanceLvL;
    std::tm estimatedTime;
    std::tm deadline;
    Status status;
    double score;
    
public:
    // constructor
    TaskManager(std::string, int, std::string, std::string, Status);

    // display function
    void showTask() const; 

    // getters function 
    int getId() const;
    std::string getStatus() const;
    std::string getTask() const;
    int getImportanceLvL() const;
    std::string getEstimatedTime() const;
    std::string getDeadline() const;
    double getScore() const;

    // setters function
    static void setNextId(int); // NEW: To reset the global counter
    void setId(int);   
    void setScore(double);    
    void setStatus(Status);
    void setTask(std::string);
    void setImportanceLvL(int);
    void setEstimatedTime(std::string);
    void setDeadline(std::string); 
};

#endif