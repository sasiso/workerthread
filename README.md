"# workerthread" 

Sometimes, we need an extra thread to do a task for us in parallel or to just unblock main thread. For better performance, we would not like to create a new thread every time. Also, we need little more control over the thread. In this article, we will implement a thread with the below abilities.

You need not define a thread procedure, rather you can submit any function using lambda
Implemented purely in C++11, compatible with any OS
Submit a task to be executed asynchronously
Submit a task to be executed synchronously (but on worker thread)
Automatically wait for task to complete
Implementation uses C++ 11 extensively (thread, lambda, condition variables)