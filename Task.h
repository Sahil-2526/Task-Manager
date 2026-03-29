#ifndef TASK_GUARD  // checks if header is defined already or not (guard)
// protect file from importing multiple times
#define TASK_GUARD // if not defined this line defines guard

#include <string>
#include <ctime>

class TaskManager {
private:
    static int nextId;
    int id;
    std::string task;
    int importanceLvL;
    std::tm estimatedTime;
    std::tm deadline;
    std::string status;
    std::tm stringToTime(std::string);

public:
    TaskManager(std::string,int,std::string,std::string,std::string);
    void showTask();
    int addTask(std::string);

    //getters function
    int getId();
    std::string getStatus();
    std::string getTask();
    int getImportanceLvL();
    std::string getEstimatedTime();
    std::string getDeadline();
        

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