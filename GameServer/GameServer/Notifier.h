#pragma once
#include <shared_mutex>
#include <condition_variable>

using namespace std;

/************************************************************************/
/* 
	User : Seobee
	Date : 2017-12-26 20:54
	Purpose : 
			해당 영역을 벗어날 시 자동으로 락을 해제 해 주기 위하여 unique_lock을 사용.
			해당 unique_lock의 경우 single_read, single_write 이므로 접근할 수 있는 쓰레드의 갯수를 한개로 제한 ( 일반적인 동기화 )

			condition_varibale_any는 일반적인 condition_variable의 경우 mutex를 사용해야 해서, any를 사용.
*/
/************************************************************************/
class Shared_Notifier
{
private:
	shared_mutex	m_mutex;
	condition_variable_any m_condvalue;

public:
	Shared_Notifier()
	{
	}

	void Notifiy_one()
	{
		std::unique_lock<shared_mutex> uniqueLock(m_mutex);
		m_condvalue.notify_one();
	}

	void Notify_all()
	{
		std::unique_lock<shared_mutex> uniqueLock(m_mutex);
		m_condvalue.notify_all();
	}

	void Wait()
	{
		std::unique_lock<shared_mutex> uniqueLock(m_mutex);
		m_condvalue.wait(uniqueLock);
	}

	void Wait(std::function<bool ()> func)
	{
		std::unique_lock< shared_mutex > uniqueLock(m_mutex);
		m_condvalue.wait(uniqueLock, func);
	}

};