#pragma once

#include <boost\asio.hpp>
#include <unordered_map>

#include "Packet.h"
#include "Threadh.h"
#include "ProcessPacket.h"
#include "GameRoom.h"

#include "NetPlayer.h"
#include "DataStruct.h"

using namespace std;

#define MAX_PLAYER_POOL_SIZE		2000
#define MAX_DATA_NODE_SIZE			50000
static const int server_port = 50064;

/*
*	User : Seobee
*	Date : 2017-11-29 18:00
*	Purpose :
*		���Ӽ����� Class. ���� ������ �ʿ��� ���� �� �Լ��� �����ϴ� Ŭ����.
*/
class ServerMain
{
public:

	boost::asio::io_service m_io_service;
	boost::asio::ip::tcp::acceptor m_acceptor;
	boost::asio::ip::tcp::socket m_socket;


	// �ӽ� ���ӷ� �Ű�����.
	GameRoom		m_MainGameRoom;

	char			m_DBPort[32];
	char			m_DBIP[32];
	char			m_DBName[32];
	char			m_DBUser[32];
	char			m_DBPass[32];
private:

	ConcurrentQueue<QueueData>	m_bufferQueue;
	TStack<QueueData, MAX_DATA_NODE_SIZE> m_StackData;
	TStack<NetPlayer, MAX_PLAYER_POOL_SIZE> m_PlayerPool;
	unordered_map<int, NetPlayer*>	m_MapPlayerList;

	WorkerThread m_workerThread[2];
	IOThread m_ioThread;

	ProcessPacket	m_ProcessPacket;


	// ================================================= �ɹ��Լ� ���� ================================ //

	ServerMain(int port) :
		m_acceptor(m_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
		m_socket(m_io_service)
	{
		Accept();
	}

	~ServerMain()
	{
		for (int i = 0; i < m_StackData.GetMaxSize(); i++)
		{
			QueueData* pData = m_StackData.PopStack();

			if (pData)
			{
				delete pData;
			}
		}
	}
public:

	static ServerMain* GetInstance()
	{
		static ServerMain m_Instance(server_port);
		return &m_Instance;
	}
		
	bool ServerSettingFromINI();
	bool ConnectDB();

	void Accept();

	void printouttest();

	void RunIOService();

	void ThreadFunctionTest1();

	bool IsEmptyBufferQueue();

	bool PopPacketData(QueueData& data);
	void PushData(QueueData& data, int nThreadNumber);

	QueueData* PopStackNode();
	void PushStackNode(QueueData* pNode);
	 
	bool InitStackMemoryPool();
	bool InitPlayerMemoryPool();

	void PacketProcess( QueueData* pData );

	void InitThread();

	boost::asio::ip::tcp::acceptor* GetAcceptor();
	boost::asio::io_service* GetIOService();

	NetPlayer* GetPlayerNode();
	void InsertPlayerIntoMap(NetPlayer* pPlayer);
	void DeletePlayerFromMap(int nListID);

	NetPlayer* FindPlayerToListID(int nListID);
};
