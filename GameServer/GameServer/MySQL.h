#pragma once
#include<mysql.h>


class DBQueryBase
{
private:
	static const int MAX_QUERY_LENGTH = 512;

protected:
	MYSQL	m_conn;
	MYSQL*	m_connection;
	MYSQL_RES* m_result;
	MYSQL_ROW m_row;
	MYSQL_FIELD* m_field;

	char	m_query[MAX_QUERY_LENGTH];

public:

	bool TryConnectToDB(char* dbIP, char* dbUser, char* dbPassword, char* dbName, char* dbPort);
	void SetQuery(const char* query);

	void EndQuery();

};


class DBConnection : public DBQueryBase
{
private:

public:

};