#ifndef _SMARTPOINTER_H__
#define _SMARTPOINTER_H__

#include "boost/shared_array.hpp"
#include "boost/shared_array.hpp"
#include "boost/make_shared.hpp"

///< 定义智能指针
template <typename T>
struct SharePtr
{
    typedef boost::shared_ptr<T> type;
};

template <typename T>
struct ShareArrayPtr
{
    typedef boost::shared_array<T> type;
};

///< 智能指针的创建
#define MakeShared boost::make_shared

///< 定义智能指针的转换
#define StaticPointerCast boost::static_pointer_cast

#define DynamicPointerCast boost::dynamic_pointer_cast

#endif  ///< _SMARTPOINTER_H__