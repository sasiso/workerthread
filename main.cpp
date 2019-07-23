#include <iostream>
#include <chrono>
#include <thread>
#include "workerthread.h"

int main()
{
	std::cout << "Hi, Welcome to demo of worker thread demo" << std::endl;

	{
		WorkerThread thread_one;
		WorkerThread thread_two;
		// Create a lambda function and submit it
		// Note this would be blocking call and will wait until function is done
		thread_one.doSync([] {
			std::cout << "First - blocking call" << std::endl;
		});
        // Now create 100 tasks and submit to worker threads
		for (int i = 1; i < 100; i++)
		{
			auto &t = i % 2 == 0 ? thread_one : thread_two;
			if (i == 10)
				thread_one.doSync([] { std::cout << "Second - blocking call" << std::endl; });

			t.doAsync([i] {
				std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(200));
				std::cout << (i % 2 == 0 ? "thread one-" : "thread two-") << "iteration number: " << i << std::endl;
			});
		}
		thread_one.wait(); // Wait unit thread one is done
		thread_two.wait(); // Wait until thread two is done

		thread_one.doSync([] { std::cout << "Last - blocking call"; });
	}
	std::cout << "This must be last line\n";
}