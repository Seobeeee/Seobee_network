#pragma once
#include <thread>
#include "Notifier.h"

using namespace std;

class ThreadBase
{
private :
	thread m_thread;

protected:



public:

	ThreadBase()
	{
		
	}

	virtual void __ThreadMain(void* instance)
	{
		ThreadBase* pSelf = (ThreadBase*)instance;

		pSelf->Threadfunction();
	}

	void InitThreadFunction();
	void Start();	

	virtual void Threadfunction() = 0;
};

class WorkerThread : public ThreadBase
{
private:

public:

	void Threadfunction();

	void InitThread();

};

class IOThread : public ThreadBase
{
private :


public :

	void Threadfunction();
};