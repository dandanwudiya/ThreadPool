// 2023/2/25

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <iostream>
#include <functional>
#include <vector>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <thread>

namespace ThreadPool{
class Threadpool{

public:
    using Task = std::function<void()>;
    Threadpool(size_t MaxQueu = 20, size_t Thread_nums = 0)
    : Thread_nums(Thread_nums), MaxQueuesize_(MaxQueu), running(false){}

    Threadpool (const Threadpool&) = delete;
    Threadpool& operator= (const Threadpool&) = delete;

    void start();

    void stop();

    ~Threadpool(){
        if(running) stop();
    }

private:
    size_t MaxQueuesize_;

    size_t Thread_nums;

    bool running;

    std::mutex mtex;
    std::condition_variable not_Full, not_Empty;

};
}



#endif