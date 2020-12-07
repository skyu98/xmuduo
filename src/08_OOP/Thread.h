#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>

class Thread
{
public:
	Thread();
	/* 基类的析构函数应该写成虚函数
	 使用new得到的实例化子类，如果赋给基类指针并且通过其调用delete
	 将不会调用子类的析构函数，基类的虚析构函数可以解决此问题
	*/
	virtual ~Thread();

	void Start();
	void Join();

	void SetAutoDelete(bool autoDelete);

private:
	static void* ThreadRoutine(void* arg); // 线程入口只需要共享一个，定义为静态成员
	virtual void Run() = 0; // 纯虚函数。强制性要求子类实现各自的版本的执行体
	pthread_t threadId_;
	bool autoDelete_;
};

#endif // _THREAD_H_
