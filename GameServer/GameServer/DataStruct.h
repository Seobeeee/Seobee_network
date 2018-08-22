#pragma once

#include <windows.h>
#include <stdio.h>
#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>

#include "Notifier.h"

using namespace std;
using namespace Concurrency;

template <typename T>
struct ArrayDeleter
{
	void operator () (T* p)
	{
		delete[] p;
	}
};

template <typename T>
class ConcurrentQueue
{
private :
	concurrent_queue<T > m_ConQueue;
	Shared_Notifier m_notifier;

public:
	ConcurrentQueue()
	{ 
		m_ConQueue.clear();
	}

	void Push(T& data)
	{
		m_ConQueue.push(data);
		m_notifier.Notifiy_one();
	}

	bool Pop( T& data )
	{
		while (!m_ConQueue.try_pop(data))
		{
			m_notifier.Wait([this]()->bool
			{
				return !isEmpty();
			});
		}

		return true;
	}
	
	bool isEmpty()
	{
		bool retVal = false;
		retVal = m_ConQueue.empty() ? true : false;

		return retVal;
	}
};



template<typename Type, int StackSize>
class DECLSPEC_ALIGN(16) TStack
{
	// Thread Safe Stack.
private:
	SLIST_HEADER	m_Header;
	int				m_nMaxStackSize;
	long			m_lSize;

public:

	TStack()
	{
		InitializeSListHead(&m_Header);
		m_nMaxStackSize = StackSize;
		m_lSize = 0;
	}

	~TStack()
	{
		PSINGLE_LIST_ENTRY	entry;
		Type* pT = NULL;
		while (1)
		{
			entry = reinterpret_cast<PSINGLE_LIST_ENTRY>(InterlockedPopEntrySList(&m_Header));

			if (entry == NULL)
			{
				break;
			}

			pT = (Type*)entry;
			delete pT;
		}
	}

	Type* PopStack()
	{
		/*
		QueueData�� ����� Stack. ���� size�� 0�ϰ�쿡�� �ݺ����� ���鼭 ��ٸ�����,
		���� ���� ���� ���. �ش� Type�� new�� ������ �� �־��ְ� return ���ִ� ������� ���氡��.
		( ������ ���� ������ �������� ����. )
		*/
		Type* pT = NULL;
		PSINGLE_LIST_ENTRY		entry;
		while (1)
		{
			entry = reinterpret_cast<PSINGLE_LIST_ENTRY>(InterlockedPopEntrySList(&m_Header));
			if (entry == NULL)
			{
				return NULL;
			}
			else
			{
				InterlockedDecrement(&m_lSize);
				pT = (Type*)entry;
				return pT;
			}
		}
	}

	bool PushStack(Type* Type)
	{
		if (m_lSize >= m_nMaxStackSize)
		{
			return FALSE;
		}

		InterlockedPushEntrySList(&m_Header, (PSLIST_ENTRY)Type);
		InterlockedIncrement(&m_lSize);

		return TRUE;
	}

	int GetSize()
	{
		int nSize = (int)m_lSize;
		return nSize;
	}

	void AddStackNode(int nSize)
	{

	}

	int GetMaxSize()
	{
		return m_nMaxStackSize;
	}
};

/*
template<typename T_Key, typename T_Value>
class ConcurrentMap
{
private:
	concurrent_unordered_map<T_Key, T_Value>	m_ConqMap;

public:

	T_Value FindEnterRoom();



};
*/
