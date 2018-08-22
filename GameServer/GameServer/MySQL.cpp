#include "MySQL.h"
#include <iostream>

bool DBQueryBase::TryConnectToDB(char* dbIP, char* dbUser, char* dbPassword, char* dbName, char* dbPort)
{
	int nPort = atoi(dbPort);

	mysql_init(&m_conn);

	m_connection = mysql_real_connect(&m_conn, "1111", dbUser, dbPassword, dbName, nPort, NULL, 0);

	if (m_connection == NULL)
	{
		const char* errorString = mysql_error(&m_conn);
		std::cout << "error is : " << errorString << std::endl;

		return FALSE;
	}
	

	return TRUE;
}

void DBQueryBase::SetQuery(const char* query)
{
	if (!query)
	{
		return;
	}

	int length = strlen(query);
	memcpy(m_query, query, length);
}

void DBQueryBase::EndQuery()
{
	memset(m_query, 0, sizeof(m_query));
}