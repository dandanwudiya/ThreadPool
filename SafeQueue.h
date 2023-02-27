//2023/2/27
//实现一个线程安全的任务队列
#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H

#include <mutex>
#include <queue>


template<typename T>
class SafeQueue
{
private:
    std::queue<T> m_queue;  //泛型任务队列

    std::mutex mtex;      //
public:
    SafeQueue(){}
    ~SafeQueue(){}

    bool empty const(){ //返回任务队列是否为空
        std::unique_lock<std::mutex> lock(mtex);
        return m_queue.empty();    
    }
    int size(){   //任务队列的实际任务数
        std::unique_lock<std::mutex> lock(mtex);
        return m_queue.size();   
    }
    void _push_task(T& t){ //添加任务
        std::unique_lock<std::mutex> lock(mtex);
        m_queue.emplace_back(t);
    }
    void _pop_tack(){    //取出任务
        std::unique_lock<std::mutex> lock(mtex);
        t = std::move(m_queue.front()); // 取出队首元素，返回队首元素值，并进行右值引用
        m_queue.pop(); // 弹出入队的第一个元素
        return true;
    }

};

#endif