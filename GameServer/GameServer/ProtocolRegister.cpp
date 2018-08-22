#include "ProtocolRegister.h"
#include <iostream>

std::map<UInt32, std::function<void(QueueData*, Protocol::ProtocolSharedPtr)>> ProtocolRegister::_processProtocols;
std::map<UInt32, Protocol::ProtocolConstructorSharedPtr> ProtocolRegister::_protocolConstructors;

ProtocolRegister::ProtocolRegister(UInt32 protocolID, std::function<void(QueueData*, Protocol::ProtocolSharedPtr)> func)
{
#if DEBUG
	if ( _processProtocols.find(protocolID) != _processProtocols.end() )
	{
		std::cout << "ProtocolRegister(), Duplicate Protocol ID" << std::endl;
		return;
	}
#endif
	_processProtocols.insert(std::make_pair(protocolID, func));

}

void ProtocolRegister::RegisterProtocolConstruector(UInt32 protocolID, Protocol::ProtocolConstructorSharedPtr constructor)
{
#if DEBUG
	if ( _protocolConstructors.find(protocolID) != _protocolConstructors.end() )
	{
		std::cout << "RegisterProtocolConstruector(), Duplicate Protocol ID" << std::endl;
		return;
	}
#endif
	_protocolConstructors.insert(std::make_pair(protocolID, constructor));
}

void ProtocolRegister::Call(UInt32 protocolID, QueueData* data, Protocol::ProtocolSharedPtr protocol)
{
#if DEBUG
	if ( _processProtocols.find(protocolID) == _processProtocols.end() )
	{
		std::cout << "Cannot find protocol function" << std::endl;
		return;
	}
#endif
	_processProtocols[protocolID](data, protocol);
}

Protocol::ProtocolSharedPtr ProtocolRegister::CreateProtocol(UInt32 protocolID)
{
#if DEBUG
	if ( _protocolConstructors.find(protocolID) == _protocolConstructors.end() )
	{
		std::cout << "Cannot find protocol constructor" << std::endl;
		return nullptr;
	}
#endif
	return Protocol::ProtocolSharedPtr(_protocolConstructors[protocolID]->CreateProtocol());
}



