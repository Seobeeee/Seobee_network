#include "PlayerData.h"

void PlayerData::InitPlayer()
{
	m_roomNumber = 0;
}

void PlayerData::SetRoom(int nRoomNumber, int nRoomListID)
{
	m_roomNumber = nRoomNumber;
	m_roomListID = nRoomListID;
}

void PlayerData::ExitRoom()
{
	m_roomNumber = 0;
	m_roomListID = 0;
}

int PlayerData::GetRoomListID()
{
	return m_roomListID;
}