# 1.注意ThreadLocal的数据结构
一旦定义一个ThreadLocal对象，由于数据空间共享，之后所有的线程都有一个key；但是只有调用了value()的线程，其key才是真正指向了对象的，并且可以通过返回的引用对对象进行操作。

# 2.value()函数返回值的思考
```cpp
 T& value()
  {
    T* perThreadValue = static_cast<T*>(pthread_getspecific(pkey_));      
    if (!perThreadValue) {
      T* newObj = new T();                       // 堆上数据
      pthread_setspecific(pkey_, newObj);       
      perThreadValue = newObj;
    }
    return *perThreadValue;    
  }
```
value()函数中动态分配了一个newObj指针，将它复制给一个perThreadValue指针变量，通过解引用perThreadValue返回T对象的引用。这样算不算是返回局部对象的引用呢？
    
1.这样是不算的。不能返回局部对象的引用/指针，本质原因是它引用的/指向的是**一个储存在栈内存的变量，在函数结束后马上会被销毁**。引用/指向未知的内存当然是危险的。
2.另一个角度想，分配在其他内存的对象是不是可以返回它的引用/指针呢？目前看来是这样，如下表：

|内存区|储存对象|生命周期|
|----|----|----|
|栈内存|局部非静态对象|运行到代码块时分配内存，离开代码块时销毁|
|静态内存|局部static成员、类static成员、定义在任何函数之外的变量|编译时分配内存，程序结束时销毁|
|动态内存（堆）|动态对象（new，智能指针）|运行到代码块时分配内存，程序结束时销毁|

储存在静态内存或动态内存的对象，我们就可以安全地返回其引用或指针。**但是对于new分配的动态对象，要注意在合适的地方delete。**

# 3.对于单例的再次说明
一定要注意Singleton类中那个静态成员才是单例。