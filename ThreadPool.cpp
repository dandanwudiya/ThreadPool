#include "ThreadPool.h"
#include <stdexcept>
#include <assert.h>

bool ThreadPool::Threadpool::isFull() const{
    return task_.size() == MaxQueuesize_;
}

//向线程池中添加任务
void ThreadPool::Threadpool::addToWorkingPool(Task task){
    if(!running){
        throw std::runtime_error("ThreadPool must running");
    }

    std::unique_lock<std::mutex> lock(mtex);

    //用while的原因是防止虚假唤醒
    while(isFull() && running){
        not_Full.wait(lock);  //等待任务队列有位置，有的话条件变量会发通知；
    }

    if(!running){
        return ;
    }

    task_.emplace_back(move(task));
    not_Empty.notify_one();
}

//工作线程
void ThreadPool::Threadpool::consumeTask(){
    while(true){
        Task task;
        {
            std::unique_lock<std::mutex> lock(mtex);
            while (task_.empty() && running) //没有任务的时候需要阻塞等待生产者添加任务
            {
                not_Empty.wait(lock);
                /* code */
            }
            if(!running && task_.empty()){
                return ;
            }

            assert(task_.size());   //出错方便调试
            task = task_.front();
            task_.pop_front();
        }
        not_Full.notify_one();
        task();
    }
}

void ThreadPool::Threadpool::start(){
    running = true;
    for(size_t i = 0; i < Thread_nums; i++){
        workers_.emplace_back(std::thread(&ThreadPool::Threadpool::consumeTask, this));
    }
}

void ThreadPool::Threadpool::stop(){
    if(!running){
        throw std::runtime_error ("threadpool not start and need't close");
    }
    {
        std::unique_lock<std::mutex> lock(mtex);
        running = false; //线程池如果需要关闭的话，工作线程应该全部阻塞,改变状态加通知。
        not_Empty.notify_all();
        not_Full.notify_all();
    }
    //之前已经通知过生产线程和消费线程了，所以可以直接回收
    for(auto& t_hread : workers_){
        t_hread.join();
    }
}

