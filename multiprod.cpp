//2023/2/28

#include "ThreadPool.h"
#include <assert.h>
#include <atomic>
#include <future>
#include <chrono>

using namespace std;
using LL = long long;
constexpr int N = 1e8, M = 50;
constexpr int workers = 8, producers = 4;
vector<future<LL>> futu[producers];
vector<promise<LL>> prom[producers];
vector<LL> result[producers];

vector<thread> prodThread;

void func(promise<LL>& p){
    this_thread::sleep_for(chrono::seconds(2));
    LL res = 0;
    for(int i = 1; i <= N; i++){
        res += i;
    }
    p.set_value(res);
}

void prod(int i, ThreadPool::Threadpool& threadpool){
    for(int j = 0; j < M; j++){
        futu[i][j] = prom[i][j].get_future();
        threadpool.addTask(func, ref(prom[i][j]));
    }
}


int main(){
    for(int i = 0; i < producers; i++){
        prom[i].resize(M);
        futu[i].resize(M);
    }

    ThreadPool::Threadpool threadpool(workers, 20);
    threadpool.start();

    prodThread.resize(producers);

    for(int i = 0; i < producers; i++){
        prodThread[i] = thread(prod, i, ref(threadpool));
    }

	// 回收生产者线程
	for (auto& t : prodThread)
		t.join();

    threadpool.stop();

	for (int i = 0; i < producers; i ++ ) {
		for (auto& f : futu[i]) {
			LL cur = f.get();
			result[i].push_back(cur);
		}
	}

	for (int i = 0; i < producers; i ++ ) {
		for (int j = 0; j < result[i].size(); j ++ ) {
			cout << i << ' ' << j << ' ' << result[i][j] << '\n';
			assert(result[i][0] == result[i][j]);
		}
	}

	return 0;
}