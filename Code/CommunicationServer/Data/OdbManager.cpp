#include "OdbManager.h"

#include "PathManager.h"
#include "LogManager.h"

#include <odb/sqlite/database.hxx>

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

template<> OdbManager* Singleton<OdbManager>::m_Singleton = 0;

OdbManager::OdbManager()
{
	///< ODB的初始化
	initialization();
}

OdbManager::~OdbManager()
{
	///< 释放用户表对象
	if (m_operUserTable != NULL)
	{
		delete m_operUserTable;
		m_operUserTable = NULL;
	}

	///< 释放数据库占用资源
	if (m_db != NULL)
	{
		delete m_db;
		m_db = NULL;
	}
}

OdbManager& OdbManager::getSingleton()
{
	assert(m_Singleton);
	return *m_Singleton;
}

OdbManager* OdbManager::getSingletonPtr()
{
	assert(m_Singleton);
	return m_Singleton;
}

bool OdbManager::logon(const std::string& account, const std::string& password)
{
	return m_operUserTable->logon(account, password);
}

bool OdbManager::Register(const std::string& account, const std::string& password, 
						  const std::string& userName, const std::string& userNamePinyin)
{
	return m_operUserTable->registerTheUser(account, password, userName, userNamePinyin);
}

const std::vector<User> OdbManager::detrainment()
{
	return m_operUserTable->detrainment();
}

bool OdbManager::initialization()
{
	///< 初始化用户表资源, 并打开用户表
	
	m_db = new odb::sqlite::database(
			PathManager::getSingleton().getOdbFile(), SQLITE_OPEN_READWRITE);
	

	///< 确保数据库加载成功.
	assert(m_db);
	LogManager::getSingleton().logDebug("Load database sucess.");

	///< 创建用户表对象
	m_operUserTable = new OperUserTable(*m_db);
	
	return true;
}

