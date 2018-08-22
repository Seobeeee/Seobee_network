#pragma once
#include <shared_mutex>
#include <condition_variable>

using namespace std;

/************************************************************************/
/* 
	User : Seobee
	Date : 2017-12-26 20:54
	Purpose : 
			�ش� ������ ��� �� �ڵ����� ���� ���� �� �ֱ� ���Ͽ� unique_lock�� ���.
			�ش� unique_lock�� ��� single_read, single_write �̹Ƿ� ������ �� �ִ� �������� ������ �Ѱ��� ���� ( �Ϲ����� ����ȭ )

			condition_varibale_any�� �Ϲ����� condition_variable�� ��� mutex�� ����ؾ� �ؼ�, any�� ���.
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