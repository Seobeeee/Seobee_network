#include "ServerMain.h"
#include "Session.h"
#include "NetPlayer.h"
#include "ClientToServer.h"
#include "ProtocolRegister.h"
#include <mysql.h>

using namespace boost::asio;
using namespace boost::asio::ip;

namespace INIFunction
{
	bool GetINIFileData(char* pSection, char* pKey, char* pBuffer, DWORD bufferSize)
	{
		char* pFileName = "/setting.ini";
		char arrFilePath[1024] = { 0, };
		GetCurrentDirectory(1024, arrFilePath);

		int result = GetPrivateProfileString(pSection, pKey, "", pBuffer, bufferSize, strcat(arrFilePath, pFileName));

		if (result)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

void ServerMain::Accept()
{
	m_acceptor.async_accept(m_socket, [this](boost::system::error_code error)
	{
		if (!error)
		{
			std::cout << " New Client Connect!! complete \n";
			make_shared<Session>(move(m_socket))->Start();
			
			// Re Call Accept function to continue server. 
			Accept();
		}
		else
		{
			// ������ �������� ��� ó���� �ʿ���. 
		}
	});
}


void ServerMain::printouttest()
{
	std::cout << "print test " << std::endl;
}

void ServerMain::RunIOService()
{
	if (!InitStackMemoryPool())
	{
		cout << " InitstackMemoryPool function Failed!!" << endl;
		return;
	}

	if (!InitPlayerMemoryPool())
	{
		cout << " InitPlayerMemoryPool Function Failed!!" << endl;
		return;
	}

	if (!ServerSettingFromINI())
	{
		cout << " Init init File Function Failed!!" << endl;
		return;
	}

	if (!ConnectDB())
	{
		cout << " DBConnection Failed!!" << endl;
		return;
	}

	InitThread();
}

void ServerMain::ThreadFunctionTest1()
{
	cout << "this is real Thread Function Test1 " << endl;
}

bool ServerMain::IsEmptyBufferQueue()
{
	return m_bufferQueue.isEmpty();
}

void ServerMain::PushData(QueueData& data, int nThreadNumber)
{
	m_bufferQueue.Push(data);
}

bool ServerMain::PopPacketData(QueueData& pData)
{
	bool retVal = m_bufferQueue.Pop(pData);
	return retVal;
}

QueueData* ServerMain::PopStackNode()
{
	if (m_StackData.GetSize() <= 0)
	{
		return nullptr;
	}
	else
	{
		QueueData* pData = m_StackData.PopStack();
		return pData;
	}
}

void ServerMain::PushStackNode(QueueData* pNode)
{
	if (pNode == nullptr)
	{
		return;
	}

	//pNode->packet.InitPacketData();
	m_StackData.PushStack(pNode);
}

bool ServerMain::InitStackMemoryPool()
{
	for (int i = 0; i < m_StackData.GetMaxSize(); i++)
	{
		QueueData* pData = new QueueData();
		if ( !m_StackData.PushStack(pData))
		{
			return FALSE;
		}
	}

	return TRUE;
}

bool ServerMain::InitPlayerMemoryPool()
{
	for (int i = 0; i < MAX_PLAYER_POOL_SIZE; i++)
	{
		NetPlayer* pPlayer = new NetPlayer( nullptr );
		m_PlayerPool.PushStack(pPlayer);
	}

	return true;
}

void ServerMain::PacketProcess( QueueData* pData )
{
	if (pData == NULL)
	{
		return;
	}

	// pData�� �����Ͱ� �̻��ϴ�.
	// Session recv���� �����͸� �о pData�� ������ ������ �ϰ� �� ��,
	// ��ť�� QueueData�� ���� �� QueueData�� ���簡 ��,
	// StreamBase��.. �ϴ� shared_ptr�� �����Ͽ� �����ǵ��� �ϰ� ��.

	Protocol::StreamReader reader(pData->packetData.get());
	int size = 0;
	reader.Read(size); // ��Ŷ ������ ����. ������ ��ȯ��.

	int pkID = reader.Peek(); // 4����Ʈ �ܾ��, ������ ��ȯ ���� ����.
	
	// shared_ptr�� ������ �ٿ�, ������ refcount�� �ͱ۾� ���� �͵�...
	Protocol::ProtocolSharedPtr protocol = ProtocolRegister::CreateProtocol(pkID);

	if ( protocol != nullptr )
	{
		protocol->Read(reader);
		ProtocolRegister::Call(pkID, pData, protocol);
	}
}

void ServerMain::InitThread()
{
	for (int i = 0; i < 2; i++)
	{
		m_workerThread[i].InitThreadFunction();
	}

	m_ioThread.InitThreadFunction();

	for (int i = 0; i < 2; i++)
	{
		m_workerThread[i].Start();
	}

	m_ioThread.Start();
}

boost::asio::ip::tcp::acceptor* ServerMain::GetAcceptor()
{
	return &m_acceptor;
}

boost::asio::io_service* ServerMain::GetIOService()
{
	return &m_io_service;
}

void ServerMain::InsertPlayerIntoMap(NetPlayer* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	int nListID = pPlayer->GetListID();
	this->m_MapPlayerList.insert(make_pair(nListID, pPlayer));
}

void ServerMain::DeletePlayerFromMap(int nListID)
{
	if (!nListID)
	{
		return;
	}

	if (m_MapPlayerList.find(nListID) == m_MapPlayerList.end())
	{
		return;
	}
	else
	{
		auto itr = m_MapPlayerList.find(nListID);
		NetPlayer* pPlayer = (*itr).second;
		pPlayer->InitPlayer();

		m_MapPlayerList.erase(nListID);

		m_PlayerPool.PushStack(pPlayer);
	}
}

NetPlayer* ServerMain::GetPlayerNode()
{
	if (m_PlayerPool.GetSize() <= 0)
	{
		return nullptr;
	}

	return m_PlayerPool.PopStack();
}


NetPlayer* ServerMain::FindPlayerToListID(int nListID)
{
	return NULL;
}

bool ServerMain::ServerSettingFromINI()
{
	if ( !INIFunction::GetINIFileData("DB Connection", "PORT", m_DBPort, sizeof(m_DBPort)))
	{
		return FALSE;
	}

	if (!INIFunction::GetINIFileData("DB Connection", "IP", m_DBIP, sizeof(m_DBIP)))
	{
		return FALSE;
	}

	if (!INIFunction::GetINIFileData("DB Connection", "USER", m_DBUser, sizeof(m_DBUser)))
	{
		return FALSE;
	}

	if (!INIFunction::GetINIFileData("DB Connection", "PASSWORD", m_DBPass, sizeof(m_DBPass)))
	{
		return FALSE;
	}

	if (!INIFunction::GetINIFileData("DB Connection", "NAME", m_DBName, sizeof(m_DBName)))
	{
		return FALSE;
	}

	return TRUE;
}

bool ServerMain::ConnectDB()
{
	int nPort = atoi(m_DBPort);

	MYSQL mysql;
	mysql_init(&mysql);

	if (!mysql_real_connect(&mysql, m_DBIP, m_DBUser, m_DBPass, m_DBName, nPort, NULL, 0))
	{
		return FALSE;
	}

	mysql_close(&mysql);
	return TRUE;
}