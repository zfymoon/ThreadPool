//
// Created by Curie on 2020/7/11.
//

#ifndef ZHUQUE_SPINLOCK_H
#define ZHUQUE_SPINLOCK_H
#include <atomic>
class SpinLock{
private:
    std::atomic_flag _lock = ATOMIC_FLAG_INIT;
public:
    void lock(){
        while(_lock.test_and_set(std::memory_order_acquire));
    }
    void unlock(){
        _lock.clear(std::memory_order_release);
    }
};

#endif //ZHUQUE_SPINLOCK_H
