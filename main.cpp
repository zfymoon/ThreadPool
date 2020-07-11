#include <iostream>
#include "task/Executor.h"
#include "task/Task.h"

class ComputeTask:public TaskInterface{
    void run() override{
        std::cout<<"Hello "<<getId()<<std::endl;
        sleep(2);
        std::cout<<"#World "<<getId()<<"#"<<std::endl;
    }
};

class ComputeTask2:public TaskInterface{
    void run() override{
        std::cout<<"Thank "<<getId()<<std::endl;
        sleep(2);
        std::cout<<"#You "<<getId()<<"#"<<std::endl;
    }
};
int main() {
    
    Executor<ComputeTask> executor(5,true,0);
    
    
    
    executor.start();
    
    executor.post(ComputeTask());
    executor.post(ComputeTask());executor.post(ComputeTask());
   
   

    
    
    sleep(10);
    executor.close();
    
    
 
    return 0;
}
