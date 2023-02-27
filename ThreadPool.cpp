#include "ThreadPool.h"
#include <stdexcept>
#include <assert.h>

bool ThreadPool::Threadpool::isFull() const{
    return task_.size() == MaxQueuesize_;
}

//���̳߳����������
void ThreadPool::Threadpool::addToWorkingPool(Task task){
    if(!running){
        throw std::runtime_error("ThreadPool must running");
    }

    std::unique_lock<std::mutex> lock(mtex);

    //��while��ԭ���Ƿ�ֹ��ٻ���
    while(isFull() && running){
        not_Full.wait(lock);  //�ȴ����������λ�ã��еĻ����������֪ᷢͨ��
    }

    if(!running){
        return ;
    }

    task_.emplace_back(move(task));
    not_Empty.notify_one();
}

//�����߳�
void ThreadPool::Threadpool::consumeTask(){
    while(true){
        Task task;
        {
            std::unique_lock<std::mutex> lock(mtex);
            while (task_.empty() && running) //û�������ʱ����Ҫ�����ȴ��������������
            {
                not_Empty.wait(lock);
                /* code */
            }
            if(!running && task_.empty()){
                return ;
            }

            assert(task_.size());   //���������
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
        running = false; //�̳߳������Ҫ�رյĻ��������߳�Ӧ��ȫ������,�ı�״̬��֪ͨ��
        not_Empty.notify_all();
        not_Full.notify_all();
    }
    //֮ǰ�Ѿ�֪ͨ�������̺߳������߳��ˣ����Կ���ֱ�ӻ���
    for(auto& t_hread : workers_){
        t_hread.join();
    }
}

