#ifndef _SMARTPOINTER_H__
#define _SMARTPOINTER_H__

#include "boost/shared_array.hpp"
#include "boost/shared_array.hpp"
#include "boost/make_shared.hpp"

///< ��������ָ��
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

///< ����ָ��Ĵ���
#define MakeShared boost::make_shared

///< ��������ָ���ת��
#define StaticPointerCast boost::static_pointer_cast

#define DynamicPointerCast boost::dynamic_pointer_cast

#endif  ///< _SMARTPOINTER_H__