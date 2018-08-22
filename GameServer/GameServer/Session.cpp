#include "Session.h"
#include <condition_variable>
#include "Packet.h"
#include "ServerMain.h"
#include "NetPlayer.h"

void Session::Send(const Int8* pBuffer, std::size_t length, int nListID)
{
	auto self(shared_from_this());
	boost::asio::async_write(m_socket, boost::asio::buffer(pBuffer, length),
		[this, nListID, pBuffer, self](boost::system::error_code error, std::size_t length)
	{
		if (!error)
		{
			//this->handle_post_recv(nListID);
			PushNodetoSendBuffer((Int8 *)pBuffer);
		}
		else
		{
			std::cout << "this packet is error!! Send function is Failed!!";
			//this->handle_post_recv(nListID);
		}
	});
}

void Session::Start()
{
	NetPlayer* pPlayer = ServerMain::GetInstance()->GetPlayerNode();
	pPlayer->SetSession(this);

	this->handle_post_recv( pPlayer->GetListID() );
}

void Session::handle_post_recv(int nListID)
{
	auto self(shared_from_this());
	m_socket.async_read_some(boost::asio::buffer(m_pBufferPos, PACKET_HEADER_SIZE - m_RecvBufferSize),
		[this, nListID, self](boost::system::error_code error, size_t length)
	{
		if (!error)
		{
			m_RecvBufferSize += (int)length;
			m_pBufferPos = m_pBufferPos + length;
			if (m_RecvBufferSize < PACKET_HEADER_SIZE)
			{
				//m_pBufferPos = m_pBufferPos + length;
				handle_post_recv(nListID);
			}
			else
			{
				// Echo!
				int PacketSize = *(int *)m_buffer.data();
				std::cout << "Total Data Size : " << PacketSize << std::endl;

				this->handle_body_recv(PacketSize, nListID);

				//m_pBufferPos = m_buffer.data();
			}
		}
		else
		{
			cout << "handle_post_recv function Error!!()" << endl;
		}

		if ((boost::asio::error::eof == error) || (boost::asio::error::connection_reset == error))
		{
			// handle the disconnect.
			std::cout << " this client close connection and retry Recv function ! \n ";
			this->m_socket.close();
			ServerMain::GetInstance()->DeletePlayerFromMap(nListID);
		}
	});
}

void Session::handle_body_recv( size_t packetsize, int nListID )
{
	size_t needSize = packetsize - m_RecvBufferSize;
	auto self(shared_from_this());
	m_socket.async_read_some(boost::asio::buffer(m_pBufferPos, needSize),
		[this, nListID, self, packetsize](boost::system::error_code error, size_t length)
	{
		if (!error)
		{
			m_RecvBufferSize += (int)length;
			m_pBufferPos = m_pBufferPos + length;
			if (m_RecvBufferSize < packetsize )
			{
				this->handle_body_recv(packetsize, nListID);
			}
			else
			{
				std::cout << " body Recv Packet Complete!! Size : " << packetsize << endl;

				// 해당 부분에서 Queue로 Data를 밀어넣어주자.
				QueueData* pData = ServerMain::GetInstance()->PopStackNode();
				pData->pPlayer = m_pPlayer;

				pData->packetData->SetPacketData(m_buffer.data(), packetsize);
				//pData->packet.SetPacketData(packetsize, m_buffer.data());
				BufferRelocate();

				ServerMain::GetInstance()->PushData((*pData), 1);

				this->handle_post_recv(nListID);
			}
		}
		else
		{
			cout << "handle_post_body function Error!!()" << endl;
		}

		if ((boost::asio::error::eof == error) || (boost::asio::error::connection_reset == error))
		{
			// handle the disconnect.
			std::cout << " this client close connection and retry Recv function ! \n ";
			this->m_socket.close();
			ServerMain::GetInstance()->DeletePlayerFromMap(nListID);
		}
	});
}

void Session::BufferRelocate()
{
	int nPacketSize = *(int *)m_buffer.data();
	m_RecvBufferSize = 0;
	memset(m_buffer.data(), 0, sizeof(char) * MAX_BUFFER_SIZE);
	m_pBufferPos = m_buffer.data();
}

Int8* Session::GetSendBufferNode()
{
	if (m_sendBufferqueue.isEmpty())
	{
		return nullptr;
	}

	Int8* data = nullptr;
	m_sendBufferqueue.Pop(data);

	return data;
}


void Session::PushNodetoSendBuffer(Int8* data)
{
	if (!data)
	{
		return;
	}

	memset(data, 0, sizeof(Int8) * MAX_PACKET_DATA_SIZE);
	m_sendBufferqueue.Push(data);
}

std::string Session::GetPlayerIP()
{
	return m_socket.remote_endpoint().address().to_string();
}

