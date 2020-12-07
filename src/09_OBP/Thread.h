#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>
#include <boost/function.hpp>

class Thread
{
public:
	typedef boost::function<void ()> ThreadFunc;
	explicit Thread(const ThreadFunc& func);

	void Start();
	void Join();

	void SetAutoDelete(bool autoDelete);

private:
	static void* ThreadRoutine(void* arg);
	void Run();  // Thread现在是具体类，不再需要虚函数；而是在Run()当中调用传入的func_()
	ThreadFunc func_;
	pthread_t threadId_;
	bool autoDelete_;
};

#endif // _THREAD_H_
