#include "Thread.h"
#include <unistd.h>
#include <iostream>
using namespace std;

class TestThread : public Thread
{
public:
	TestThread(int count) : count_(count)
	{
		cout<<"TestThread ..."<<endl;
	}

	~TestThread()
	{
		cout<<"~TestThread ..."<<endl;
	}

private:
	/* 虚函数具有回调的功能 */
	void Run()
	{
		while (count_--)
		{
			cout<<"this is a test ..."<<endl;
			sleep(1);
		}
	}

	int count_;
};

int main(void)
{
	/*
	TestThread t(5);
	t.Start();
	// t.Run()  此处不能直接调用Run()，否则是在主线程运行，而不是创建一个新线程并回调它

	t.Join();   // 线程在此处被销毁，而TestThread对象没有
	*/	

	TestThread* t2 = new TestThread(2);
	t2->SetAutoDelete(true);
	t2->Start();
	/* 这里有一个问题：如果在上一步中t2已经被delete了，它又怎么去调用Join呢？
		应该将它放在delete之前
	*/
	//t2->Join();      
	

	for (; ; )
		pause();

	return 0;
}
