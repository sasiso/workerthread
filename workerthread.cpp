#include "workerthread.h"


WorkerThread::WorkerThread() :m_isRunning(false)
{
	// Start the thread
	m_thread.reset(new std::thread([this]
	{
		m_isRunning = true;
		this->startThread();
	}));
}

WorkerThread::~WorkerThread()
{
	stop();	
}

void WorkerThread::startThread()
{
	std::unique_lock<std::mutex> l(m_mutex);
	do
	{
		while (m_isRunning && m_tasks.empty())
			m_itemInQueue.wait(l); // wait on mutex to unless a task is added

		while (!m_tasks.empty())
		{
			auto size = m_tasks.size();
			printf("Number of pending task are %ld\n", size);
			const std::function<void()> t = m_tasks.front();
			m_tasks.pop_front();
			l.unlock(); // Let other task be queued until we compete this task
			t();
			l.lock();
		}
		m_itemInQueue.notify_all();


	} while (m_isRunning);
	m_itemInQueue.notify_all();
}

void WorkerThread::doAsync(const std::function<void()>& t)
{
	std::lock_guard<std::mutex> _(m_mutex);
	m_tasks.push_back(t);
	m_itemInQueue.notify_one();

}

void WorkerThread::doSync(const std::function<void()>& t)
{
	std::condition_variable event;
	bool finished = false;

	std::unique_lock<std::mutex> l(m_mutex);
	auto lambda = [this, &t, &finished, &event]
	{
		t();
		std::lock_guard<std::mutex> l(m_mutex);

		finished = true;
		event.notify_one();
	};
	m_tasks.push_back(lambda);
	m_itemInQueue.notify_one();

	while (!finished)
		event.wait(l);

}

void WorkerThread::wait()
{
	std::unique_lock<std::mutex> l(m_mutex);
	while (!m_tasks.empty())
		m_itemInQueue.wait(l);
}

void WorkerThread::stop()
{
	{
		std::lock_guard<std::mutex> l(m_mutex);
		m_isRunning = false;
		m_itemInQueue.notify_one();
	}
	m_thread->join();
}