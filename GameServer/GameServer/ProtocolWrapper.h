#pragma once
#include "Protocol.h"
#include "ProtocolRegister.h"

class ProtocolWrapper
{
public:
	// 클라이언트에게 전송할 sendData를 생성한다.
	// 앞에 4바이트는 데이터 사이즈
	// 중복 생성을 방지할 필요가 있을 것 같은데..(protocol의 데이터가 바뀌었다거나..)
	const Int8* GenerateSendDataForClient(bool forceGenerate = false)
	{
		if ( forceGenerate || _writer.GetSize() == 0 )
		{
			size_t size = 0;
			bool result = true;

			result &= _writer.Write(size);
			if ( !result ) return nullptr;

			result &= _protocol->Write(_writer);
			if ( !result ) return nullptr;

			size = _writer.GetSize();

			result &= _writer.Write(&size, 0, sizeof(size));
			if ( !result ) return nullptr;
		}

		return GeRawtBuffer();
	}

	size_t GetDataSize()
	{
		return _writer.GetSize();
	}

	const Int8* GeRawtBuffer()
	{
		return _stream.GetBuffer();
	}

	inline Protocol::ProtocolBase* GetProtocol() const
	{
		return _protocol.get();
	}

protected:
	ProtocolWrapper() : _writer(&_stream)
	{

	}

	Protocol::StreamBase _stream;
	Protocol::StreamWriter _writer;
	Protocol::ProtocolSharedPtr _protocol;
};

template <typename ProtocolType>
class GenericProtocolWrapper : public ProtocolWrapper
{
public:
	GenericProtocolWrapper() 
	{
		_protocol = ProtocolRegister::CreateProtocol(ProtocolType::__ID__);
	}

	operator ProtocolType*( ) const
	{
		return (ProtocolType*)GetProtocol();
	}

	ProtocolType* operator->() const
	{
		return (ProtocolType*)GetProtocol();
	}
};
