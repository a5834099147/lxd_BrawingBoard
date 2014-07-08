#include "odb_user.h"

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

odb::database* odb_user::db = NULL;

odb_user::odb_user(void)
{
    initialization();
}


odb_user::~odb_user(void)
{
}

void odb_user::initialization()
{
    if (db == NULL)
    {
        db = new odb::sqlite::database(
            "test.db", SQLITE_OPEN_READWRITE);
    }
    ///< 确保数据库加载成功.
    assert(db);
}

bool odb_user::logon(const std::string& account, const std::string& password)
{
    typedef odb::query<User> query;
    typedef odb::result<User> result;

    transaction t(db->begin());

    result r(db->query<User>(query::account == account && query::password == password));

    return !r.empty();
}

bool odb_user::registerTheUser(const std::string& account, const std::string& password, 
    const std::string& userName, const std::string& userNamePinyin)
{
    if (registerCheck(account))
    {
        ///< 查询到有该用户名的用户存在, 注册失败
        return false;
    }

    User user(account, password, userName, userNamePinyin);
    transaction t(db->begin());
    db->persist(user);
    t.commit();
    return true;
}

const std::vector<User> odb_user::detrainment()
{
    typedef odb::query<User> query;
    typedef odb::result<User> result;

    transaction t(db->begin());
    result r(db->query<User> (query::id > 0));

    std::vector<User> user_v;

    for (result::iterator i(r.begin()); i != r.end(); ++i)
    {
        User user(i->Account(), "保密", i->UserName(), i->UserNamePinyin());
        user_v.push_back(user);
    }
    return user_v;
}

const std::vector<User> odb_user::fuzzySearch(std::string str)
{
    typedef odb::query<User> query;
    typedef odb::result<User> result;

    transaction t(db->begin());
    result r(db->query<User> (query::user_name_pinyin.like(str += "%")));

    std::vector<User> user_v;

    for (result::iterator i(r.begin()); i != r.end(); ++i)
    {
        User user(i->Account(), "保密", i->UserName(), i->UserNamePinyin());
        user_v.push_back(user);
    }
    return user_v;
}

bool odb_user::registerCheck( const std::string& account )
{
    typedef odb::query<User> query;
    typedef odb::result<User> result;

    transaction t(db->begin());

    result r(db->query<User>(query::account == account));

    return !r.empty();
}




