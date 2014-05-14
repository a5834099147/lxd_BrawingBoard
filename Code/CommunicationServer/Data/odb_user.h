#pragma once
#include <memory>
#include <iostream>
#include <vector>
#include <string>

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>

#include <odb/sqlite/database.hxx>

#include "user.hxx"
#include "user-odb.hxx"

using namespace std;
using namespace odb::core;

class odb_user
{
public:
	odb_user(void);
	~odb_user(void);

	bool static logon(const std::string& account, const std::string& password);	
	void static registerTheUser(const std::string& account, const std::string& password, 
						const std::string& userName, const std::string& userNamePinyin);
	const std::vector<User> static detrainment();
	const std::vector<User> static fuzzySearch(std::string str);
private:
	void static initialization();
	static odb::database* db;
};

