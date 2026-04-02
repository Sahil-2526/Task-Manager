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
    double progress;
    int importanceLvL;
    std::tm estimatedTime;
    std::tm deadline;
    Status status;
    double score;
    
    // Auto Schedule Variables
    std::tm scheduledDate;
    bool hasSchedule;
    
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
    double getProgress() const;
    
    bool getHasSchedule() const;
    std::string getScheduledDate() const;
    std::tm getScheduledDateTm() const;

    // setters function
    static void setNextId(int); 
    void setId(int);   
    void setScore(double);    
    void setStatus(Status);
    void setTask(std::string);
    void setImportanceLvL(int);
    void setEstimatedTime(std::string);
    void setDeadline(std::string); 
    void setProgess(double);
    
    void setScheduledDate(std::string date);
    void clearScheduledDate();
};

#endif