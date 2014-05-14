#include "odb_user.h"

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
}

bool odb_user::logon(const std::string& account, const std::string& password)
{
	typedef odb::query<User> query;
	typedef odb::result<User> result;

	transaction t(db->begin());

	result r(db->query<User>(query::account == account && query::password == password));

	return !r.empty();
}

void odb_user::registerTheUser(const std::string& account, const std::string& password, 
	                               const std::string& userName, const std::string& userNamePinyin)
{
	User user(account, password, userName, userNamePinyin);
	transaction t(db->begin());
	db->persist(user);
	t.commit();
}

const std::vector<User> odb_user::export()
{
	typedef odb::query<User> query;
	typedef odb::result<User> result;

	transaction t(db->begin());
	result r(db->query<User> (query::id > 0));

	std::vector<User> user_v;

	for (result::iterator i(r.begin()); i != r.end(); ++i)
	{
		User user(i->Account(), "±£ÃÜ", i->UserName(), i->UserNamePinyin());
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
		User user(i->Account(), "±£ÃÜ", i->UserName(), i->UserNamePinyin());
		user_v.push_back(user);
	}
	return user_v;
}




