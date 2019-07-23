#pragma once
#include <functional>
#include <thread>
#include <list>
#include <mutex>
#include <memory>
#include <condition_variable>

class WorkerThread
{

public:
	WorkerThread();
	~WorkerThread();
	void doAsync(const std::function<void()>& t);

	
	void doSync(const std::function<void()>& t);

	void wait();
	void stop();

private:
	void startThread();
private:
	std::condition_variable m_itemInQueue;
	std::mutex m_mutex; // to avoid race condtions
	std::unique_ptr<std::thread> m_thread; // Hold a thread
	std::list<std::function<void()>> m_tasks;
	volatile bool m_isRunning;
};