#ifndef USER_HXX
#define USER_HXX

#include <odb/core.hxx>
#include <string>

#pragma  warning(disable:4068)

#pragma db object
class User
{
public:
    User(const std::string& account, const std::string& password, 
        const std::string& user_name, const std::string& user_name_pinyin)
        : account_(account), password_(password), user_name_(user_name),
        user_name_pinyin_(user_name_pinyin)
    {}

    const int Id()
        const
    {
        return id_;
    }

    const std::string Account()
        const
    {
        return account_;
    }

    void Account(const std::string& account)
    {
        account_ = account;
    }

    const std::string Password()
        const
    {
        return password_;
    }

    void Password(const std::string& password)
    {
        password_ = password;
    }

    const std::string UserName()
        const
    {
        return user_name_;
    }

    void UserName(const std::string& user_name)
    {
        user_name_ = user_name;
    }

    const std::string UserNamePinyin()
        const 
    {
        return user_name_pinyin_;
    }

    void UserNamePinyin(const std::string& user_name_pinyin)
    {
        user_name_pinyin_ = user_name_pinyin;
    }


private:	
    User() {}

    friend class odb::access;

#pragma db id auto
    int id_;

    std::string account_;
    std::string password_;
    std::string user_name_;
    std::string user_name_pinyin_;
};

#endif
