#pragma once
#include <windows.h>

/*
	User : Seobee
	Date : 2018-01-04
	Purpose : 
			Critical_Section의 Enter 및 Leave가 귀찮아!! 그래서 만든 클래스.
			EnterCriticalSection을 호출 후 별도로 Leave를 호출하지 않아도 해당 객체가 소멸시 LeaveCriticalSection을 호출하도록 구현.
*/

class SDCritical_Section
{
private:
	CRITICAL_SECTION		m_cs;


public:
	SDCritical_Section()
	{
		InitializeCriticalSectionAndSpinCount(&m_cs, 1000);
	}

	~SDCritical_Section()
	{
		LeaveCriticalSection(&m_cs);
	}

	void Critical_Section_Enter()
	{
		EnterCriticalSection(&m_cs);
	}
};