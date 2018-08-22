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
*    Session에 대한 정보를 다룰 클래스. 해당 클래스에서 Recv 및 Send 함수를 관리한다.
*    Player에 대한 정보는 NetPlayer Class 에서 관리 할 예정.
*    enable_shared_from_this 상속은 자기 자신을 shared_ptr로 받았을 경우 refcount가 증가하지 않는 것을 증가시켜주기 위하여 상속.
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
		// TCP의 특성상 네이글 알고리즘 기능이 기본으로 on이므로, 실시간 서버에서는 패킷량이 많을것으로 판단.
		// 따라서 해당 알고리즘(nodelay)을 off상태로 만들어서 패킷을 뭉쳐서 보내지 않도록 만들어 준다. 변경될 수 있음.
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
		이벤트를 활용한 Packet Recv 방법. ( 읽어오는 사이즈를 구분하여 패킷을 쪼갠다 )
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
	*	Send버퍼를 다루게 될 함수
	*/
	Int8* GetSendBufferNode();
	void PushNodetoSendBuffer(Int8* data);


	/**
	*
	*	User : Seobee
	*	Date : 2018-02-05
	*	Purpose : 
	*	유저의 IP를 가져오는 함수.
	*/
	std::string GetPlayerIP();
};