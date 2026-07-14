// 60-concurrency — do several things at once, safely.
#include <atomic>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
    // 1. Threads run concurrently; join() waits for each to finish.
    //    std::cout is shared, so a mutex serializes access to it.
    std::mutex io_mutex;
    auto worker = [&](int id) {
        std::lock_guard<std::mutex> lock(io_mutex);
        std::cout << "thread " << id << " working\n";
    };
    std::vector<std::thread> pool;
    for (int i = 0; i < 3; ++i) pool.emplace_back(worker, i);
    for (auto& t : pool) t.join();

    // 2. std::atomic: a shared counter two threads can bump with no data race.
    std::atomic<int> counter{0};
    auto bump = [&] { for (int i = 0; i < 1000; ++i) ++counter; };
    std::thread a(bump), b(bump);
    a.join();
    b.join();
    std::cout << "counter = " << counter << " (expected 2000)\n";

    // 3. std::async runs a task and hands back a future for its result.
    std::future<int> answer = std::async(std::launch::async, [] { return 6 * 7; });
    std::cout << "async result = " << answer.get() << '\n';
    return 0;
}
