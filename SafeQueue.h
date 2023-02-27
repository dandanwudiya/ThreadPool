//2023/2/27
//ʵ��һ���̰߳�ȫ���������
#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H

#include <mutex>
#include <queue>


template<typename T>
class SafeQueue
{
private:
    std::queue<T> m_queue;  //�����������

    std::mutex mtex;      //
public:
    SafeQueue(){}
    ~SafeQueue(){}

    bool empty const(){ //������������Ƿ�Ϊ��
        std::unique_lock<std::mutex> lock(mtex);
        return m_queue.empty();    
    }
    int size(){   //������е�ʵ��������
        std::unique_lock<std::mutex> lock(mtex);
        return m_queue.size();   
    }
    void _push_task(T& t){ //�������
        std::unique_lock<std::mutex> lock(mtex);
        m_queue.emplace_back(t);
    }
    void _pop_tack(){    //ȡ������
        std::unique_lock<std::mutex> lock(mtex);
        t = std::move(m_queue.front()); // ȡ������Ԫ�أ����ض���Ԫ��ֵ����������ֵ����
        m_queue.pop(); // ������ӵĵ�һ��Ԫ��
        return true;
    }

};

#endif