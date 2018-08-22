#pragma once
#include <tchar.h>
#include "NetPlayer.h"
#include "Protocol/Protocol.h"

#define MAX_PACKET_DATA_SIZE 1024
#define DEFAULT_PACKET_BYTES 8
#define PACKET_HEADER_SIZE 4

/*
* User : Seobee
* Date : 2017-11-27 14:56
* Purpose :
*		��Ŷ�� Ŭ����. �ش���Ŷ Ŭ������ �̿��Ͽ� binary ��Ŷ�� �����Ͽ� Ŭ���̾�Ʈ���� ���� �� �ش�.
*		Packet�� �������� ���� �� SetID�� ���Ͽ� �ش� ��Ŷ�� Header( 4 bytes )�� ���� �� Ŭ���̾�Ʈ���� ����
*/
class Packet
{
private:
	int m_Size;
	char m_Data[MAX_PACKET_DATA_SIZE];

	char* m_pHeader;
	char* m_pBody;
	char* m_pPopPoz;
public:
	Packet()
	{
		// Header ������ ( int 4 bytes)+ ��Ŷ ������ ( int 4bytes )
		InitPacketData();
	}

	void InitPacketData()
	{
		m_Size = DEFAULT_PACKET_BYTES;
		m_pHeader = m_Data;
		memset(m_Data, 0, sizeof(char) * MAX_PACKET_DATA_SIZE);

		m_pBody = m_Data + PACKET_HEADER_SIZE + PACKET_HEADER_SIZE;
		m_pPopPoz = m_pBody;
	}

	/*
		User : Seobee
		Date : 2017-12-14 10:30
		Purpose : 
				��Ŷ �����͸� �������ִ� �Լ�.

	*/
	void SetPacketData(int nPacketSize, char* pData)
	{
		m_Size = nPacketSize;

		memcpy(m_Data, &nPacketSize, sizeof(int));
		memcpy(m_Data + sizeof(int), pData + 4, nPacketSize - sizeof(int));
	}

	char* getData()
	{
		return m_Data;
	}

	int getPacketHeader()
	{
		int nType = *(int *)(m_Data + PACKET_HEADER_SIZE);
		return nType;
	}

	int getPacketSize()
	{
		return m_Size;
	}

	// --------------------------- Packet Insert ------------------------- //
	inline void SetID(const int HeaderType)
	{
		int* pHeaderType = (int*)(m_Data + PACKET_HEADER_SIZE);

		*pHeaderType = HeaderType;
	}

	inline void operator << (const int data)
	{
		memcpy(m_Data + m_Size, &data, sizeof(int));
		m_Size += sizeof(int);
	}

	inline void operator << (const double data)
	{
		memcpy(m_Data + m_Size, &data, sizeof(double));
		m_Size += sizeof(double);
	}

	inline void operator << (const float data)
	{
		memcpy(m_Data + m_Size, &data, sizeof(float));
		m_Size += sizeof(float);
	}

	inline void operator << (const char data)
	{
		memcpy(m_Data + m_Size, &data, sizeof(char));
		m_Size += sizeof(char);
	}

	inline void operator << (const char* data)
	{
		int length = (int)strlen(data);
		memcpy(m_Data + m_Size, data, sizeof(char) * length);
		m_Size += sizeof(char) * length;
	}

	inline void operator << (const bool data)
	{
		memcpy(m_Data + m_Size, &data, sizeof(bool));
		m_Size += sizeof(bool);
	}

	// ------------------------ Packet Pop ------------------------------- //

	inline void operator >> (int& data)
	{
		memcpy(&data, m_pPopPoz, sizeof(int));
		m_pPopPoz += sizeof(int);
	}

	inline void operator >> (double& data)
	{
		memcpy(&data, m_pPopPoz, sizeof(double));
		m_pPopPoz += sizeof(double);
	}

	inline void operator >> (float& data)
	{
		memcpy(&data, m_pPopPoz, sizeof(float));
		m_pPopPoz += sizeof(float);
	}

	inline void operator >> (char& data)
	{
		memcpy(&data, m_pPopPoz, sizeof(char));
		m_pPopPoz += sizeof(char);
	}
	
	inline void operator >> (char* data)
	{
		char* pText = strtok(m_pPopPoz, "\n");
		memcpy(data, pText, sizeof(char) * strlen(pText));
		m_pPopPoz += strlen(pText);
	}
	
	inline void operator >> (bool data)
	{
		memcpy(&data, m_pPopPoz, sizeof(bool));
		m_pPopPoz += sizeof(bool); 
	}
};

class Netplayer;
class QueueData
{
public :
	QueueData()
	{
		packetData.reset(new Protocol::StreamBase());
	}
	

	// ��� ���� �Ǵ��� Ȯ���Ϸ��� �Ʒ� �ּ� Ǯ��, �� �Ʒ� ���Կ����� �ּ�ó���� ��.
	//QueueData& operator=(const QueueData& rhs) = delete;
	QueueData& operator=(const QueueData& rhs)
	{
		entry = rhs.entry;
		pPlayer = rhs.pPlayer;
		packetData = rhs.packetData;
		return *this;
	}

	QueueData(const QueueData& rhs)
	{
		entry = rhs.entry;
		packetData = rhs.packetData;
		pPlayer = rhs.pPlayer;
	}

	SLIST_ENTRY entry;
	NetPlayer* pPlayer; 

	//����� ���� ���� ������ ���� ����� ���Ͽ� �����ͷ� ����.	
	std::shared_ptr<Protocol::StreamBase> packetData;
};
