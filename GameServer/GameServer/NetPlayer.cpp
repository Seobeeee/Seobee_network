#include "Session.h"
#include "NetPlayer.h"
#include "Packet.h"
#include <array>
#include "ProtocolWrapper.h"

void NetPlayer::SetListID(int nListID)
{
	m_listID = nListID;
}

int NetPlayer::GetListID()
{
	return m_listID;
}

SLIST_ENTRY* NetPlayer::GetListEntry()
{
	return &m_entry;
}

void NetPlayer::InitPlayer()
{
	m_pSession = NULL;
}

void NetPlayer::SetSession(Session* pSession)
{
	m_pSession = pSession;
	pSession->m_pPlayer = this;
}

void NetPlayer::Send(Packet* pk)
{
	/*
	std::array<char, 1024> SendBuffer = { 0, };
	int nHeader = pk->getPacketHeader();
	int nSize = pk->getPacketSize();

	memcpy(SendBuffer.data(), &nSize, sizeof(int));
	memcpy(SendBuffer.data() + 4, &nHeader, sizeof(int));
	memcpy(SendBuffer.data() + 8, pk->getData(), nSize - 8);

	m_pSession->Send(SendBuffer.data(), nSize, m_listID);
	*/
}

void NetPlayer::Send(ProtocolWrapper* stream)
{
	Int8* sendBuffer = m_pSession->GetSendBufferNode();
	const Int8* buffer = stream->GenerateSendDataForClient();
	size_t offsetSize = stream->GetDataSize();

	memcpy(sendBuffer, buffer, offsetSize);

	m_pSession->Send(sendBuffer, (int)offsetSize, m_listID);
}