#include "GameRoom.h"
#include "NetPlayer.h"
#include "PlayerData.h"
#include "Packet.h"

void GameRoom::InitRoom()
{
	m_playerCount = 0;
	m_RoomNumber = 0;
	m_RoomType = eRoomType::ROOM_TYPE_NONE;
}

GameRoom* GameRoom::EnterRoom(NetPlayer* pPlayer)
{
	if (!pPlayer)
	{
		return nullptr;
	}

	if (m_playerCount >= MAX_ROOM_MEMBER_COUNT )
	{
		return nullptr;
	}

	m_cs.Critical_Section_Enter();

	for (int i = 1; i < MAX_GAME_ROOM_NUMBER; i++)
	{
		if (!m_PlayerList[i])
		{
			m_PlayerList[i] = pPlayer;
			pPlayer->m_PlayerData.SetRoom(m_RoomNumber, i);
			++m_playerCount;
			return this;
		}
	}

	return nullptr;
}

bool GameRoom::ExitRoom(NetPlayer* pPlayer)
{
	if (!pPlayer)
	{
		return FALSE;
	}
	
	PlayerData* pData = &pPlayer->m_PlayerData;

	int nRoomListID = pData->GetRoomListID();
	pData->ExitRoom();

	--m_playerCount;

	//m_cs.Critical_Section_Enter();
	m_PlayerList[nRoomListID] = NULL;
	
	return TRUE;
}

void GameRoom::BroadCastPacket(ProtocolWrapper* protocol, NetPlayer* pPlayer)
{
	for (int i = 1; i < MAX_ROOM_MEMBER_COUNT; i++)
	{
		if (pPlayer == m_PlayerList[i])
		{
			continue;
		}

		if (m_PlayerList[i])
		{
			m_PlayerList[i]->Send(protocol);
		}
	}
}