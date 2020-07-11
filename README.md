# ThreadPool
## 使用

```
class ComputeTask:public TaskInterface{
    void run() override{
        std::cout<<"Hello "<<getId()<<std::endl;
        sleep(2);
        std::cout<<"#World "<<getId()<<"#"<<std::endl;
    }
};

Executor<ComputeTask> executor(5,true,0);
executor.start();
executor.post(ComputeTask());
```
## 任务队列的竞争，通过atomic_flag实现spin lock
```
void post(T task){
        while(taskQueueFlag.test_and_set(std::memory_order_acquire)){
        }
        if(_state == RUN ){
            taskQueue.push(task);
        }
        taskQueueFlag.clear(std::memory_order_release);
    }
```
