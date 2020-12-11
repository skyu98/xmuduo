# 1.assert(obj == t_value_); 报错
这里报错是完全没有想到的。
假设线程1中，```pkey```关联的对象是```Test1```，按道理，```obj```应该指向Test1，```t_value_```也在```setspecific()```中被设定为指向Test1。
那么在```destructor()```中，```assert(obj == t_value_);```应该就是正确的。

最后发现，在```destructor()```被系统自动调用之前，```t_value_```不知道怎么被设置为```NULL```了，这样```assert(obj == t_value_);```就会报错。
**如果在线程退出之前手动调用```destructor()```，两者相等，断言为真。**


# 2.改正
所以在```destructor()```中，应该```delete obj;```而不是```delete t_value_```，否则```Test1```的析构函数不会被调用，内存没有被正确释放，造成内存泄漏。

# 3.ThreadLocalSingleton<T> 和 Singleton<ThreadLocal<T>>的区别
前者是每一个线程，都有一份自己的单例T* t_value_。可以通过各自的这个指针，去访问各自的对象。线程间通过一个```static deleter 和 static pkey```来管理。
后者是所有线程共享一个单例ThreadLocal<T>，通过其中的```static pkey```去访问各自的对象。