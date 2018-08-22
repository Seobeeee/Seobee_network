#include "ServerMain.h"
#include "ProcessPacket.h"
#include "GameRoom.h"
#include "Packet.h"
#include "NetPlayer.h"
#include "ClientToServer.h"
#include "Session.h"

using namespace Protocol;


// ���Ƿ� ���⿡ ����صξ��µ�, ��� ��Ƽ� ó���ؾ��� ���� �ʿ��غ���.
#include "ProtocolRegister.h"
#include "ClientToServer.h"
#include "ProtocolWrapper.h"

ProcessPacket::ProcessPacket()
{
	// ��... �Լ� ���ε��Ǵ� ���������� �׷��� ġ��, ������ 2�� ���������� ��� ���� �������..
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

	// ���� ���� ��û �� �������� ���۵Ǵ� ��Ŷ
	GenericProtocolWrapper<Protocol::CharacterEnterMatchingRequestReply> sendPacket;

	GameRoom* pGameRoom = ServerMain::GetInstance()->m_MainGameRoom.EnterRoom(pPlayer);
	if(!pGameRoom)
	{
		sendPacket->result = (Int8)2;
		pPlayer->Send(&sendPacket);
		return;
	}

	// ���� �� �������� ���۵Ǵ� ��Ŷ.
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

	// ����� �濡 �����ϴ� �����鿡�� ���۵Ǵ� ��Ŷ.
	GenericProtocolWrapper<Protocol::ReplyNotifyCharacterEnterRoom> broadcastPacket;
	broadcastPacket->result = 1;
	broadcastPacket->playerGSN = pData->GetPlayerGSN();
	broadcastPacket->playerCSN = pData->GetPlayerCSN();
	broadcastPacket->publicIP = pPlayer->m_pSession->GetPlayerIP();
	broadcastPacket->listID = pPlayer->GetListID();
	broadcastPacket->nickName = "abcdefgg";

	pGameRoom->BroadCastPacket(&broadcastPacket, pPlayer);	
}
