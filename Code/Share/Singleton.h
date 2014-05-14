#ifndef _SINGLETON_H__
#define _SINGLETON_H__

#include <cassert>

///< 模板类实现单件类的创建
template <typename T>
class Singleton
{
private:
    ///< 禁止复制构造函数
    Singleton(const Singleton<T>&);

    ///< 禁止赋值构造函数
    Singleton& operator = (const Singleton<T> &);

protected:
    static T* m_Singleton;

public:
    Singleton()
    {
        assert(!m_Singleton);
        m_Singleton = static_cast<T*>(this);
    }

    ~Singleton()
    {
        assert(m_Singleton);
        m_Singleton = 0;
    }

    static T& getSingleton(void)
    {
        assert(m_Singleton);
        return (*m_Singleton);
    }

    static T* getSingletonPtr(void)
    {
        return m_Singleton;
    }
};

#endif ///< _SINGLETON_H__