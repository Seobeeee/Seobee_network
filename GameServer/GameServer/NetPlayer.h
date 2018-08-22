#pragma once
#include "PlayerData.h"
#include <stdio.h>
#include <windows.h>

class Session;
class Packet;
class ProtocolWrapper;

class NetPlayer 
{
private:
	SLIST_ENTRY m_entry;
	int m_listID;


public:
	NetPlayer(Session* pSession)
	{
		m_listID = 0;
		m_pSession = pSession;
	}

	Session* m_pSession;
	PlayerData m_PlayerData;

	void InitPlayer();
	void SetListID(int nListID);
	int GetListID();
	SLIST_ENTRY* GetListEntry();

	void SetSession(Session* pSession);

	void Send(Packet* pk);
	void Send(ProtocolWrapper* stream);
};
