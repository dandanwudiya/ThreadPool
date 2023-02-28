//2023/2/27
//生产者线程，用作测试

#include "ThreadPool.h"
#include <assert.h>
#include <future>
#include <chrono>

using namespace std;
using LL = long long;
constexpr int N = 1e6, M = 200;
constexpr int workers = 8;
vector<future<LL>> futu(M);
vector<promise<LL>> prom(M);
vector<LL> result;

void func(promise<LL>& p){
	LL res = 0;
	for(int i = 1; i <= N; i++){
		res += i;
	}
	p.set_value(res);
}

int main() {
	ThreadPool::Threadpool threadPool(workers, 20);

	threadPool.start();

	for (int i = 0; i < M; i ++ ) {
		// 添加到任务队列中
		futu[i] = prom[i].get_future();
		threadPool.addTask(func, ref(prom[i])); //一定要记得要用std::ref() 包装
	}

	// 结束线程池
	threadPool.stop();

	for (auto& f :futu) {
		auto cur = f.get();
		result.push_back(cur);
	}

	for (int i = 0; i < M; i ++ ) {
		cout << i << ' ' << result[i] << '\n';
		assert(result[0] == result[i]);
	}

	return 0;
}