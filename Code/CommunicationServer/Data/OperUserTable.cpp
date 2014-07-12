#include "OperUserTable.h"


#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>

#include <odb/sqlite/database.hxx>

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")


OperUserTable::OperUserTable(odb::database& db)
:m_userTable(db)
{

}


OperUserTable::~OperUserTable(void)
{
	
}

bool OperUserTable::logon(const std::string& account, const std::string& password)
{
	///< 定义结果类型以及查询类型
    typedef odb::query<User> query;
    typedef odb::result<User> result;

	///< 事务的创建
    transaction t(m_userTable.begin());

	///< 查询条件 
    result r(m_userTable.query<User>(query::account == account && query::password == password));

    return !r.empty();
}

bool OperUserTable::registerTheUser(const std::string& account, const std::string& password, 
    const std::string& userName, const std::string& userNamePinyin)
{
    if (registerCheck(account))
    {
        ///< 查询到有该用户名的用户存在, 注册失败
        return false;
    }

	///< 创建一个 User 对象
    User user(account, password, userName, userNamePinyin);
    transaction t(m_userTable.begin());
	///< 将对象提交给数据库
    m_userTable.persist(user);
    t.commit();
    return true;
}

const std::vector<User> OperUserTable::detrainment()
{
    typedef odb::query<User> query;
    typedef odb::result<User> result;

    transaction t(m_userTable.begin());
    result r(m_userTable.query<User> (query::id > 0));

    std::vector<User> user_v;

    for (result::iterator i(r.begin()); i != r.end(); ++i)
    {
		///< 将结果封装为实体
        User user(i->Account(), "保密", i->UserName(), i->UserNamePinyin());
        user_v.push_back(user);
    }
    return user_v;
}

const std::vector<User> OperUserTable::fuzzySearch(std::string str)
{
    typedef odb::query<User> query;
    typedef odb::result<User> result;

    transaction t(m_userTable.begin());
	///< 模糊查询
    result r(m_userTable.query<User> (query::user_name_pinyin.like(str += "%")));

    std::vector<User> user_v;

    for (result::iterator i(r.begin()); i != r.end(); ++i)
    {
        User user(i->Account(), "保密", i->UserName(), i->UserNamePinyin());
        user_v.push_back(user);
    }
    return user_v;
}

bool OperUserTable::registerCheck( const std::string& account )
{
    typedef odb::query<User> query;
    typedef odb::result<User> result;

    transaction t(m_userTable.begin());

    result r(m_userTable.query<User>(query::account == account));

    return !r.empty();
}




