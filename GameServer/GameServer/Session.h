#pragma once

#include <iostream>
#include <boost\asio.hpp>
#include "Protocol.h"
#include "DataStruct.h"
#include "Packet.h"

#define PACKET_HEADER_SIZE 4

static const int this_server_flag = 1;
using namespace std;
using namespace boost::asio;

class NetPlayer;


/*
* User : Seobee
* Date : 2017-11-29 16:42
* Purpose : 
*    Session�� ���� ������ �ٷ� Ŭ����. �ش� Ŭ�������� Recv �� Send �Լ��� �����Ѵ�.
*    Player�� ���� ������ NetPlayer Class ���� ���� �� ����.
*    enable_shared_from_this ����� �ڱ� �ڽ��� shared_ptr�� �޾��� ��� refcount�� �������� �ʴ� ���� ���������ֱ� ���Ͽ� ���.
*/
class Session : public enable_shared_from_this<Session>
{
private :
	ConcurrentQueue<Int8 *> m_sendBufferqueue;

public:

	enum eBufferSize
	{
		MAX_BUFFER_SIZE = 10240,
	};

	array<char, MAX_BUFFER_SIZE> m_buffer;
	char* m_pBufferPos;
	int m_RecvBufferSize;

	boost::asio::ip::tcp::socket m_socket;
	NetPlayer* m_pPlayer;

public:
	Session(boost::asio::ip::tcp::socket socket) : m_socket(std::move(socket))
	{
		// TCP�� Ư���� ���̱� �˰��� ����� �⺻���� on�̹Ƿ�, �ǽð� ���������� ��Ŷ���� ���������� �Ǵ�.
		// ���� �ش� �˰���(nodelay)�� off���·� ���� ��Ŷ�� ���ļ� ������ �ʵ��� ����� �ش�. ����� �� ����.
		m_socket.set_option(boost::asio::ip::tcp::no_delay(false));

		memset(m_buffer.data(), 0, sizeof(char) * MAX_BUFFER_SIZE);
		m_pBufferPos = m_buffer.data();
		m_RecvBufferSize = 0;

		for (int i = 0; i < 20; i++)
		{
			Int8* data = new Int8[MAX_PACKET_DATA_SIZE];
			memset(data, 0, sizeof(Int8) * MAX_PACKET_DATA_SIZE);
			m_sendBufferqueue.Push(data);
		}
	}

	~Session()
	{
		while ( !m_sendBufferqueue.isEmpty())
		{
			Int8* data = nullptr;

			m_sendBufferqueue.Pop(data);
			if (data)
			{
				delete[] data;
			}
		}
	}

	void Send(const Int8* pBuffer, std::size_t length, int nListID);

	void Start();

	/************************************************************************/
	/*
		User : Seobee
		Date : 2017-12-04
		Purpose :
		�̺�Ʈ�� Ȱ���� Packet Recv ���. ( �о���� ����� �����Ͽ� ��Ŷ�� �ɰ��� )
	*/
	/************************************************************************/
	void handle_post_recv( int nListID );
	void handle_body_recv( size_t packetsize, int nListID );

	void BufferRelocate();

	/**
	*
	*	User : Seobee
	*	Date : 2018-02-02
	*	Purpose : 
	*	Send���۸� �ٷ�� �� �Լ�
	*/
	Int8* GetSendBufferNode();
	void PushNodetoSendBuffer(Int8* data);


	/**
	*
	*	User : Seobee
	*	Date : 2018-02-05
	*	Purpose : 
	*	������ IP�� �������� �Լ�.
	*/
	std::string GetPlayerIP();
};