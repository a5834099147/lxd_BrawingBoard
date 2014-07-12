#ifndef __OdbManager_H__
#define __OdbManager_H__

#include "SmartPointer.h"
#include "Singleton.h"
#include "OperUserTable.h"

#include <odb/database.hxx>

#include "user.hxx"

class OdbManager : public Singleton<OdbManager>
{
public:
	OdbManager();

	~OdbManager();

	///< 获取单件类的实例对象
	static OdbManager& getSingleton();

	///< 获取单件类的实例指针
	static OdbManager* getSingletonPtr();

public:
	///< 登陆数据库的查询
	bool logon(const std::string& account, const std::string& password);

	///< 注册
	bool Register(const std::string& account, const std::string& password,
		const std::string& userName, const std::string& userNamePinyin);

	///< 导出数据表中的所有数据
	const std::vector<User> detrainment();

private:
	///< 初始化各个数据库表
	bool initialization();

private:
	///< 用户表
	OperUserTable* m_operUserTable;
	
	///< 数据库表对象
	odb::database* m_db;
};




#endif //__OdbManager_H__