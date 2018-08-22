#pragma once
#include "CriticalSection.h"
#include "CommonDefine.h"
#include "Protocol.h"
#include "ProtocolWrapper.h"

class NetPlayer;
class Packet;

class GameRoom
{
public:
	enum eRoomType
	{
		ROOM_TYPE_NONE = 0,
		ROOM_TYPE_GAME = 1,
		ROOM_TYPE_COMMUNITY = 2,

		ROOM_TYPE_GM = 100,
	};

	enum eRoomErrorType
	{
		ROOM_ERROR_TYPE_NONE = 0,
		ROOM_SUCCESS = 1,

		ROOM_MEMBER_COUNT_FULL = 2,
	};

private:
	SDCritical_Section	m_cs;

	int m_RoomNumber;
	char m_RoomType;
	int m_MaxRating;
	int m_MinRating;
	int m_AvgRating;
	Int8 m_playerCount;

public:

	NetPlayer* m_PlayerList[MAX_GAME_ROOM_NUMBER];

public:

	GameRoom(int roomNumber, char roomType) : m_RoomNumber(roomNumber), m_RoomType(roomType)
	{
		InitRoom();
	}

	GameRoom()
	{
		InitRoom();
	}

	void InitRoom();
	GameRoom* EnterRoom(NetPlayer* pPlayer);
	bool ExitRoom(NetPlayer* pPlayer);

	/*
		User : Seobee
		Date : 2018-01-05
		Purpose :
				인자로 넘어오는 유저를 제외한 모든 유저에게 해당 패킷을 전송한다.
	*/
	void BroadCastPacket(ProtocolWrapper* protocol, NetPlayer* pPlayer = NULL);

	Int8 GetPlayerCount()
	{
		return m_playerCount;
	}



};