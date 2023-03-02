#include "ThreadPool.h"
#include <assert.h>
#include <future>
#include <chrono>

using namespace std;
using LL = long long;

constexpr int N = 1e8, M = 200;

vector<promise<LL>> promOneThread(M);
vector<future<LL>> futuOneThread(M);

vector<promise<LL>> promThreadPool;
vector<future<LL>> futuThreadPool;

vector<LL> resultOneThread;

vector<LL> resultThreadpool;


void func(promise<LL>& P){  //用来单线程运算
    LL res = 0LL;
    for(int i = 1; i <= N; i++){
        res += i;
    }
    P.set_value(res);
}

void oneThreadRun(){
    std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::system_clock::now();
    for(int i = 0; i < M; i++){
        futuOneThread[i] = promOneThread[i].get_future();
        func(ref(promOneThread[i]));
        LL cur = futuOneThread[i].get();
        resultOneThread.push_back(cur);
    }

    for(int i = 0; i < M; i++){
        assert(resultOneThread[0] == resultOneThread[i]);
    }
	std::chrono::time_point<std::chrono::system_clock> end_time = std::chrono::system_clock::now();
	std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	cout << "\n oneThreadRun cost time = " << time.count() << " ms\n";
}

void threadPoolRun(){
	std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::system_clock::now();

    promThreadPool.resize(M);
    futuThreadPool.resize(M);

    ThreadPool::Threadpool threadpool(8, 20);

    threadpool.start();

    for(int i = 0; i < M; i++){
        futuThreadPool[i] = promThreadPool[i].get_future();
        threadpool.addTask(func, ref(promThreadPool[i]));
    }

    threadpool.stop();

    for(auto& f : futuThreadPool){
        auto cur = f.get();
        resultThreadpool.push_back(cur);
    }

    for(int i = 0; i < M; i++){
        assert(resultThreadpool[i] == resultThreadpool[0]);
    }
    std::chrono::time_point<std::chrono::system_clock> end_time = std::chrono::system_clock::now();
	std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	cout << "\n ThreadPoolRun cost time = " << time.count() << " ms\n";
}


int main(){
    oneThreadRun();
    threadPoolRun();
    return 0;
}
