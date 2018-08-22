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
	// �������� ���� ó���� �Լ��� �ּҸ� ���ε��Ͽ� �����ϴ� �����̳�.
	static std::map<UInt32, std::function<void(QueueData*, Protocol::ProtocolSharedPtr)>> _processProtocols;

	// ���������� ID ������ ���������� �����ϱ� ���Ͽ�, 
	// ���������� ������ �� �ִ� ProtocolConstructor*�� �����ϴ� �����̳�
	// ��� register�� ������ ���̳�
	static std::map<UInt32, Protocol::ProtocolConstructorSharedPtr> _protocolConstructors; 
protected:
	ProtocolRegister() {};
	ProtocolRegister(UInt32 protocolID, std::function<void(QueueData*, Protocol::ProtocolSharedPtr)> func);

	void RegisterProtocolConstruector(UInt32 protocolID, Protocol::ProtocolConstructorSharedPtr constructor);
public:
	// �������� ID�� ���� ���ε��� Process �Լ��� �����ϴ� �Լ�.
	static void Call(UInt32 protocolID, QueueData* data, Protocol::ProtocolSharedPtr);

	// �������� ID�� �̿��Ͽ� ���������� �����ϴ� �Լ�.
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
