#ifndef MUDUO_BASE_COPYABLE_H
#define MUDUO_BASE_COPYABLE_H

namespace muduo
{

/// A tag class emphasises the objects are copyable.
/// The empty base class optimization applies.
/// Any derived class of copyable should be a value type.
class copyable   // 空基类；用于标识该类为可拷贝类型
{
};

};

#endif  // MUDUO_BASE_COPYABLE_H
