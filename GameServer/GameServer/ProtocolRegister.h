#pragma once
#include "Packet.h"
#include "Protocol.h"
#include <map>
#include <functional>
#include <memory>

#define PROTOCOL_REGISTER_WITH_BIND(Protocol, BindFunction) \
{ GenericProtocolRegister<Protocol> v(BindFunction);}

#define PROTOCOL_REGISTER(Protocol) \
{ GenericProtocolRegister<Protocol> v;}

class QueueData;
class ProtocolRegister
{
	// 프로토콜 별로 처리할 함수의 주소를 바인딩하여 저장하는 컨테이너.
	static std::map<UInt32, std::function<void(QueueData*, Protocol::ProtocolSharedPtr)>> _processProtocols;

	// 프로토콜의 ID 값으로 프로토콜을 생성하기 위하여, 
	// 프로토콜을 생성할 수 있는 ProtocolConstructor*를 저장하는 컨테이너
	// 얘랑 register랑 구조가 꼬이네
	static std::map<UInt32, Protocol::ProtocolConstructorSharedPtr> _protocolConstructors; 
protected:
	ProtocolRegister() {};
	ProtocolRegister(UInt32 protocolID, std::function<void(QueueData*, Protocol::ProtocolSharedPtr)> func);

	void RegisterProtocolConstruector(UInt32 protocolID, Protocol::ProtocolConstructorSharedPtr constructor);
public:
	// 프로토콜 ID에 따라 바인딩된 Process 함수를 실행하는 함수.
	static void Call(UInt32 protocolID, QueueData* data, Protocol::ProtocolSharedPtr);

	// 프로토콜 ID를 이용하여 프로토콜을 생성하는 함수.
	static Protocol::ProtocolSharedPtr CreateProtocol(UInt32 protocolID);
};

template <typename ProtocolType>
class GenericProtocolRegister : public ProtocolRegister
{
public:
	GenericProtocolRegister()
	{
		RegisterProtocolConstruector(ProtocolType::__ID__, ProtocolType::RegisterClass());
	}

	GenericProtocolRegister(std::function<void(QueueData*, Protocol::ProtocolSharedPtr)> func) : ProtocolRegister(ProtocolType::__ID__, func)
	{
		RegisterProtocolConstruector(ProtocolType::__ID__, ProtocolType::RegisterClass());
	}
};
