#include "Thread.h"
#include <iostream>
using namespace std;


Thread::Thread() : autoDelete_(false)
{
	cout<<"Thread ..."<<endl;
}

Thread::~Thread()
{
	cout<<"~Thread ..."<<endl;
}

void Thread::Start()
{
	/* int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
	显然这里的start_routine并不是成员函数指针，它没有this指针，所以不能直接传入Run
	全局函数可以解决此问题，但暴露给了用户；为了隐藏，我们使用一个static的private成员函数来作为入口，在其中调用Run()
	*/
	pthread_create(&threadId_, NULL, ThreadRoutine, this);
}	

void Thread::Join()
{
	cout << "Join() ..." << endl;	
	pthread_join(threadId_, NULL);
}

void* Thread::ThreadRoutine(void* arg)
{
	/*ThreadRoutine并没有this指针，我们怎么去调用非静态成员函数Run()呢？
	我们利用其形参arg，将this指针传入，就可以通过其调用Run()了
	*/	
	Thread* thread = static_cast<Thread*>(arg);  // static-cast进行基本的指针类型检查，将传入的派生类指针转换为基类指针
	thread->Run();
	thread->Join(); // delete之前Join()，就能确保正常回收进程
	if (thread->autoDelete_) // Run()完检查是否需要自动释放线程对象
		delete thread;
	return NULL;
}

void Thread::SetAutoDelete(bool autoDelete)
{
	autoDelete_ = autoDelete;
}
