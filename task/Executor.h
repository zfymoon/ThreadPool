//
// Created by Curie on 2020/7/11.
//

#ifndef ZHUQUE_EXECUTOR_H
#define ZHUQUE_EXECUTOR_H
#include <queue>
#include <thread>
#include <ctime>
#include <unistd.h>
#include "Task.h"
#include <map>
#include <atomic>

/**
 * 线程池
 * coreSize
 * time 存活时间
 * fixed 是否固定线程数目
 */
typedef enum {
    NEW,
    RUN,
    DESTROY
} ExecutorState;

template <typename T >
class Executor{
private:
    ExecutorState _state = NEW;
    unsigned _coreThreadCount = 0;
    bool _isFixedSize = false;
    unsigned long _expireTime = 0;
    std::queue<std::thread *> threadQueue;
    std::queue<T> taskQueue;
    std::atomic_flag threadFlag = ATOMIC_FLAG_INIT;
    std::atomic_flag taskQueueFlag= ATOMIC_FLAG_INIT;
public:
    typedef enum {
        TASK_NEW,
        TASK_RUN,
        TASK_STOP
    }TaskState;
class TaskThread: public TaskInterface,public std::thread {
    private:
        Executor * _pool;
        std::time_t _updateTime;
        TaskState _taskState;
        static void runTask(TaskThread * taskThread){
            taskThread->run();
        }
    public:
        explicit TaskThread(Executor * executor):std::thread(runTask, this){
            _pool = executor;
            _taskState = TASK_NEW;
            _updateTime = getCurrentTime();
        }
        static std::time_t getCurrentTime(){
            return time(nullptr);
        }
        void run() override{
            _taskState = TASK_RUN;
            while(_pool->_state == RUN && _taskState == TASK_RUN ){
                while(_pool->taskQueueFlag.test_and_set(std::memory_order_acquire)){
                    std:sched_yield();
                }
                if(!_pool->taskQueue.empty()){
                    TaskInterface & task = _pool->taskQueue.front();
                    std::cout<<"Get lock"<<std::endl;
                    _pool->taskQueue.pop();
                    _pool->taskQueueFlag.clear(std::memory_order_release);
                    task.run();
                    _updateTime = getCurrentTime();
                }else if(!_pool->_isFixedSize){
                    _pool->taskQueueFlag.clear(std::memory_order_release);
                    if(getCurrentTime() > _pool->_expireTime + _updateTime){
                        _taskState = TASK_STOP;
                    } else {
                        sleep(_pool->_expireTime);
                    }
                } else {
                    _pool->taskQueueFlag.clear(std::memory_order_release);
                }
            }
        }
    };
    Executor(unsigned corePoolSize, bool isFixed, unsigned expireTime){
        _state = NEW;
        _isFixedSize = isFixed;
        _coreThreadCount = corePoolSize;
        _expireTime = expireTime;
    }
    void start(){
        _state = RUN;
        if(_coreThreadCount != 0) {
            if (!threadFlag.test_and_set()) {
                if(threadQueue.empty()) {
                    for (int i = 0; i < _coreThreadCount; i++) {
                        auto * threadPointer = new TaskThread(this);
                        threadQueue.push(threadPointer);
                    }
                }
                threadFlag.clear();
            }
        }
    }
    void post(T task){
        while(taskQueueFlag.test_and_set(std::memory_order_acquire)){
        }
        if(_state == RUN ){
            taskQueue.push(task);
        }
        taskQueueFlag.clear(std::memory_order_release);
    }
    void close(){
        _state = DESTROY;
        if (!threadFlag.test_and_set()) {
            if (!threadQueue.empty()) {
                while(!threadQueue.empty()){
                    auto  threadItem = threadQueue.front();
                    threadQueue.pop();
                    threadItem->detach();
                }
            }
        }
    }
};


#endif //ZHUQUE_EXECUTOR_H
