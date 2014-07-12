#pragma once
#include <memory>
#include <iostream>
#include <vector>
#include <string>

#include <odb/database.hxx>

#include "user.hxx"
#include "user-odb.hxx"

using namespace std;
using namespace odb::core;

class OperUserTable
{
public:
	OperUserTable(odb::database& db);
    ~OperUserTable(void);

	///< 注册用户
	bool registerTheUser(const std::string& account, const std::string& password,
		const std::string& userName, const std::string& userNamePinyin);

	///< 用户登录
    bool logon(const std::string& account, const std::string& password);    

	///< 将用户表信息导出
    const std::vector<User> detrainment();

	///< 模糊查询
    const std::vector<User> fuzzySearch(std::string str);

private:
	///< 注册检查
	bool registerCheck(const std::string& account);	

	OperUserTable operator=(OperUserTable const&);

private:
    ///< 数据库表对象
    odb::database& m_userTable;
};

