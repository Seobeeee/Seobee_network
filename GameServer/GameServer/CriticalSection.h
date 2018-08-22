#pragma once
#include <windows.h>

/*
	User : Seobee
	Date : 2018-01-04
	Purpose : 
			Critical_Section�� Enter �� Leave�� ������!! �׷��� ���� Ŭ����.
			EnterCriticalSection�� ȣ�� �� ������ Leave�� ȣ������ �ʾƵ� �ش� ��ü�� �Ҹ�� LeaveCriticalSection�� ȣ���ϵ��� ����.
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