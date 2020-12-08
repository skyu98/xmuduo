// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_ATOMIC_H
#define MUDUO_BASE_ATOMIC_H

#include <boost/noncopyable.hpp>
#include <stdint.h>

namespace muduo
{

namespace detail
{
template<typename T>  // 模版类
class AtomicIntegerT : boost::noncopyable
{
 public:
  AtomicIntegerT()
    : value_(0)
  {
  }

  //通过把拷贝构造函数和赋值运算符声明为私有，来保证不能被拷贝；也可以将其声明为delete
  // uncomment if you need copying and assignment        
  //
  // AtomicIntegerT(const AtomicIntegerT& that)
  //   : value_(that.get())
  // {}
  //
  // AtomicIntegerT& operator=(const AtomicIntegerT& that)
  // {
  //   getAndSet(that.get());
  //   return *this;
  // }

  T get()
  {
    return __sync_val_compare_and_swap(&value_, 0, 0); // 原子操作：获取value_（等于0设置为0，也是获取value_）
  }

  T getAndAdd(T x)
  {
    return __sync_fetch_and_add(&value_, x);    // 原子操作：获取value_，然后加上给定的X
  }

  T addAndGet(T x)
  {
    return getAndAdd(x) + x;    // getAndAdd(x)就是add，return是Get
  }

  T incrementAndGet()
  {
    return addAndGet(1);
  }

  T decrementAndGet()
  {
    return addAndGet(-1);
  }

  void add(T x)
  {
    getAndAdd(x);
  }

  void increment()
  {
    incrementAndGet();
  }

  void decrement()
  {
    decrementAndGet();
  }

  T getAndSet(T newValue)
  {
    return __sync_lock_test_and_set(&value_, newValue); // 原子操作：获取value_，并且设置为新的value_
  }

 private:
  volatile T value_;    //避免编译器优化
};
}

/* 设置常用的类型 */
typedef detail::AtomicIntegerT<int32_t> AtomicInt32;  
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;
}

#endif  // MUDUO_BASE_ATOMIC_H
