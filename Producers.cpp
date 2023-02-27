//2023/2/27
//�������̣߳���������

#include "ThreadPool.h"
#include <assert.h>
#include <future>
#include <chrono>

using namespace std;
using LL = long long;
constexpr int N = 1e8, M = 200;
constexpr int workers = 8;
vector<future<LL>> futu(M);
vector<promise<LL>> prom(M);
vector<LL> result;


int main() {
	ThreadPool::Threadpool threadPool(workers, 20);

	threadPool.start();

	for (int i = 0; i < M; i ++ ) {
		// ��ӵ����������
		futu[i] = prom[i].get_future();
		threadPool.addTask([&] (promise<LL>& p) -> void {
				LL res = 0LL;
				for (int i = 1; i <= N; i ++ )
					res += i;
				p.set_value(res);
			}, ref(prom[i]));
	}

	// �����̳߳�
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