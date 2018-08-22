#include "Threadh.h"
#include <iostream>
#include "ServerMain.h"
#include "Session.h"

void ThreadBase::InitThreadFunction()
{
	m_thread = std::thread(&ThreadBase::__ThreadMain, this, this);
}

void ThreadBase::Start()
{
	m_thread.join();
}

void WorkerThread::Threadfunction()
{
	while (1)
	{
		QueueData packetData;
		if ( !ServerMain::GetInstance()->PopPacketData(packetData) )
		{
			std::cout << " WorkerThread Going to sleep Tonight!! " << std::endl;
		}
		else
		{
			//std::cout << " Workerthread Pop Packet Success!! " << std::endl;
			ServerMain::GetInstance()->PacketProcess(&packetData);

			ServerMain::GetInstance()->PushStackNode(&packetData);
		}
	}
}

void IOThread::Threadfunction()
{
	ServerMain::GetInstance()->GetIOService()->run();
}