#pragma once
#include <string>
#include "Protocol.h"

class PlayerData
{
private:
	Int32 m_roomNumber;
	Int32 m_roomListID;
	Int64 m_GSN;				// ���� ������ȣ
	Int64 m_CSN;				// ĳ���� ������ȣ
	std::string m_nickName;


public:
	PlayerData()
	{
		InitPlayer();
	}

	void InitPlayer();

	void SetRoom(int nRoomNumber, int RoomListID);
	void ExitRoom();
	int GetRoomListID();

	Int64 GetPlayerCSN() { return m_CSN; }
	Int64 GetPlayerGSN() { return m_GSN; }
	std::string GetPlayerName() { return m_nickName; }
};
