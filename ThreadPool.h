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

    //��������ӿڣ��ӿɱ����ģ��
    template<typename A, typename... _Args>
    void addTask(A&& _f, _Args&&... __args){
        Task task = std::bind(std::forward<A>(_f), std::forward<_Args...>(__args...));
        //����bind��forward����ת���Խӿں�������װ��
        addToWorkingPool(task);
    }

private:
    void addToWorkingPool(Task task);
    bool isFull() const;

    void consumeTask();

    //�����߳�
    std::vector<std::thread> workers_;

    //�������
    std::deque<Task> task_;
    size_t MaxQueuesize_;

    size_t Thread_nums;

    bool running;

    std::mutex mtex;
    std::condition_variable not_Full, not_Empty;

};
}



#endif