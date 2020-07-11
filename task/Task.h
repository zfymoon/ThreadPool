//
// Created by Curie on 2020/7/11.
//

#ifndef ZHUQUE_TASK_H
#define ZHUQUE_TASK_H

#include <atomic>
#include <string>

class TaskInterface{

public:
    static std::atomic<unsigned > globalThreadId;
    unsigned taskId = 0;
    TaskInterface(){
        taskId = globalThreadId++;
    }
    virtual void run() = 0;
    unsigned getId(){
        return taskId;
    }
    virtual std::string getName(){
        return "Task["+std::to_string(taskId)+"]";
    }
};
std::atomic<unsigned > TaskInterface::globalThreadId(0);


#endif //ZHUQUE_TASK_H
