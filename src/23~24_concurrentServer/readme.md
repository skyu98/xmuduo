# 一、常见并发服务器
## 1.Iterative
迭代式，也叫循环式。其实不是并发服务器。一次只有一个客户端建立连接。
* 单线程
* 短连接（长连接会影响其他客户端的连接响应速度）
* 无法充分利用多核cpu

## 2.并发式服务器（单纯多线程/多进程）
**one connection per process/ one connection per thread**
* 主线程/进程负责监听
* 每次accept返回，则创建一个子线程/进程，完成读写操作
* 适合任务执行时间较长的场景
* 能力有限，因为能创建的线程/进程数有限

## 3.Pre-fork / Pre-threaded
预先创建多个子线程/进程，各自建立监听循环。
* 预先分配，减少了时间开销，响应更快
* 一个客户端连接，可能造成多个线程/进程accept返回，但只有一个返回成功。这是**惊群**现象。Linux现版本据说已经解决。

## 4.Reactor (select / poll / epoll)
可以多个客户端连接，单线程轮询多个客户端，处理其中的请求。多路IO复用。
* 并发处理多个请求，实际上是单线程，采用循环
* 短连接（长连接会影响其他客户端的处理响应速度）
* 无法充分利用多核cpu
* 并发量比单纯的并发式服务器好，因为能处理更多连接

## 5.Reactor + thread per request（过渡方案）
每有一个请求，就分配一个新的线程去处理。多个请求就需要创建多个线程，可以利用多核cpu，但是不太好     

## 6.Reactor + worker thread（过渡方案）
所有的请求都交给工作线程去处理，但是这两种还不如单纯多线程/进程的效率，还多了一个reactor步骤

## 7.Reactor + threadPool
Reactor把请求作为产生的任务，放到任务队列中，交由threadPool去处理；自己只充当事件循环监听和IO线程。
* 适应于计算密集型和时间较长的任务
* threadPool只负责处理计算任务，IO由IO线程负责

## 8.Multiple Reactors
单个Reactor在面对突发大量网络IO时，会造成瓶颈和阻塞。此时可以利用多线程/进程产生多个Reactor来处理。
* mainReactor把任务均匀分配给subReactor，是轮叫模式（Round Robin）
* subReactor单线程/进程完成事件循环和请求处理
* 一个Reactor可以处理一个千兆网口，可以据此确定线程数

## 9.Multiple Reactors + threadPool （目前最完美的模型）
**one loop per thread + threadPool**
* 既能处理突发IO，又能处理密集计算
* 只能多线程实现，因为多进程不共享线程池

## 10.Proactor服务器
基于异步IO机制的模式。（关于同步/异步IO：https://www.cnblogs.com/felixzh/p/10345929.html）
* 理论上效率比Reactor高
* 异步IO使IO操作与计算可以重叠，充分利用DMA机制，无需CPU干预
* Linux下的异步IO（都有缺陷）：
    * glibc aio(aio_* )，有bug
    * kernal native aio(io_* )：目前仅支持O_DIRECTO_DIRECT方式进行磁盘读写，跳过了系统缓存
* boost asio：实际上底层使用epoll实现，来模拟异步IO

