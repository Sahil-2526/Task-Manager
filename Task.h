#ifndef TASK_GUARD  // checks if header is defined already or not (guard)
// protect file from importing multiple times
#define TASK_GUARD // if not defined this line defines guard

#include <string>
#include <ctime> // tm

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
    void showTask() const; // FIXED: Marked as const

    // getters function (FIXED: All getters marked as const)
    int getId() const;
    std::string getStatus() const;
    std::string getTask() const;
    int getImportanceLvL() const;
    std::string getEstimatedTime() const;
    std::string getDeadline() const;
    double getScore() const;

    // setters function
    void setId(int);   
    void setScore(double);    
    void setStatus(Status);
    void setTask(std::string);
    void setImportanceLvL(int);
    void setEstimatedTime(std::string);
    void setDeadline(std::string); 
};

#endif 

// Note =>
// In abc.h is not prefered to use "using namespace std;"
// std namepsace will be included in every file in whihc its imported in -> namespace clash
// --------------------------------------------
// abc.h -> we put all declaration inside it
//          we include it using #include
// --------------------------------------------
// #include <ctime>   ->  tm ( datatype ) -> used to store time ( all value separately )
// tm time={};  -. make empty structure
// --------------------------------------------