#include <iostream>
#include "ServerMain.h"
#include "DataStruct.h"


#include "MySQL.h"
using namespace std;

#define THREAD_COUNT	2

ConcurrentQueue<char *> conQueue;

void FunctionTest()
{
	char* data = new char[1024];
	data = "this is text";

	conQueue.Push(data);
}

void main(int argc, char* argv[])
{
	cout << "this is main sentence !! start project !" << endl;

	try
	{
		if (argc != 2)
		{
			cerr << "Usage: Async_tcp_echo_server <port>\n";
			return;
		}

		DBQueryBase* pDB = new DBQueryBase();
		pDB->TryConnectToDB("127.0.0.1", "root", "audtjq1!", "test", "3306");

		delete pDB;
		//ServerMain::GetInstance()->ServerSettingFromINI();
		//ServerMain::GetInstance()->RunIOService();
	}
	catch (exception& e)
	{
		cout << "Exception: " << e.what() << "\n";
	}
}
