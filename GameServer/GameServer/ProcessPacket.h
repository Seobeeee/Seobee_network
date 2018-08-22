#pragma once
#include "PacketProtocol.h"
class QueueData;

namespace Protocol
{
class ProtocolBase;
}

class ProcessPacket
{
private:
public:
	ProcessPacket();

	static void C_TO_G_CHAR_ENTER_MATCHING_REQ(QueueData* pQueueData, Protocol::ProtocolSharedPtr protocol);	
};
