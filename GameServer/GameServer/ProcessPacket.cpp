#include "ServerMain.h"
#include "ProcessPacket.h"
#include "GameRoom.h"
#include "Packet.h"
#include "NetPlayer.h"
#include "ClientToServer.h"
#include "Session.h"

using namespace Protocol;


// 임의로 여기에 등록해두었는데, 어디 모아서 처리해야할 곳이 필요해보임.
#include "ProtocolRegister.h"
#include "ClientToServer.h"
#include "ProtocolWrapper.h"

ProcessPacket::ProcessPacket()
{
	// 흠... 함수 바인딩되는 프로토콜은 그렇다 치고, 나머지 2개 프로토콜은 어떻게 쉽게 등록하지..
	PROTOCOL_REGISTER_WITH_BIND(Protocol::CharacterEnterMatchingRequest, &ProcessPacket::C_TO_G_CHAR_ENTER_MATCHING_REQ);
	PROTOCOL_REGISTER(Protocol::CharacterEnterMatchingRequestReply);
	PROTOCOL_REGISTER(Protocol::ReplyNotifyCharacterEnterRoom);
}

void ProcessPacket::C_TO_G_CHAR_ENTER_MATCHING_REQ(QueueData* pQueueData, Protocol::ProtocolSharedPtr protocol)
{
	NetPlayer* pPlayer = pQueueData->pPlayer;
	if (!pPlayer)
	{
		return;
	}
	PlayerData* pData = &pPlayer->m_PlayerData;
	
	Int32 nListID = pPlayer->GetListID();
	Int64 playerCSN = pData->GetPlayerCSN();
	Int64 playerGSN = pData->GetPlayerGSN();
	string playerName = pData->GetPlayerName();
	Int8 roomType = 0;
	Int8 bResult = 1;

	Protocol::CharacterEnterMatchingRequest* packet = ( Protocol::CharacterEnterMatchingRequest* )protocol.get();
	roomType = packet->roomtype;

	// 방을 입장 요청 한 유저에게 전송되는 패킷
	GenericProtocolWrapper<Protocol::CharacterEnterMatchingRequestReply> sendPacket;

	GameRoom* pGameRoom = ServerMain::GetInstance()->m_MainGameRoom.EnterRoom(pPlayer);
	if(!pGameRoom)
	{
		sendPacket->result = (Int8)2;
		pPlayer->Send(&sendPacket);
		return;
	}

	// 입장 한 유저에게 전송되는 패킷.
	Int8 playerCount = pGameRoom->GetPlayerCount();
	sendPacket->result = bResult;
	sendPacket->playerCount = playerCount;	
	sendPacket->listID = pPlayer->GetListID();

	for (int i = 0; i < playerCount; i++)
	{
		NetPlayer* user = pGameRoom->m_PlayerList[i];
		if (user == pPlayer)
		{
			continue;
		}

		Int32 listID = user->GetListID();
		Int64 userCSN = user->m_PlayerData.GetPlayerCSN();
		Int64 userGSN = user->m_PlayerData.GetPlayerGSN();

		Protocol::UserInfoTest userInfo;
		userInfo.listID = listID;
		userInfo.nickName = "abcdef";
		userInfo.playerCSN = userCSN;
		userInfo.playerGSN = userGSN;
		userInfo.publicIP = user->m_pSession->GetPlayerIP();

		sendPacket->userInfoTest.push_back(userInfo);
	}

	pPlayer->Send(&sendPacket);

	// 입장시 방에 존재하는 유저들에게 전송되는 패킷.
	GenericProtocolWrapper<Protocol::ReplyNotifyCharacterEnterRoom> broadcastPacket;
	broadcastPacket->result = 1;
	broadcastPacket->playerGSN = pData->GetPlayerGSN();
	broadcastPacket->playerCSN = pData->GetPlayerCSN();
	broadcastPacket->publicIP = pPlayer->m_pSession->GetPlayerIP();
	broadcastPacket->listID = pPlayer->GetListID();
	broadcastPacket->nickName = "abcdefgg";

	pGameRoom->BroadCastPacket(&broadcastPacket, pPlayer);	
}
