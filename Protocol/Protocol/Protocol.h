#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <memory>

#define PROTOCOL_CLASS_DEF(className) public: static const char* GetProtocolName() { return #className; } \
	static ProtocolConstructorSharedPtr RegisterClass() { return ProtocolConstructorSharedPtr(new Protocol::GenericProtocolConstructor<className>); } \
	public: \
	className() { Initialize(); } \
	virtual ~className() { }

#define PROTOCOL_CLASS_DEF_WITHOUT_REGISTER(className) public: static const char* GetProtocolName() { return #className; } \
	public: \
	className() { Initialize(); } \
	virtual ~className() { }

#define STREAM_DEFAULT_BUFFER_SIZE 1024

typedef int8_t		Int8;
typedef int16_t		Int16;
typedef int32_t		Int32;
typedef int64_t		Int64;
typedef uint32_t	UInt32;
typedef uint16_t	UInt16;
typedef uint8_t		UInt8;
typedef uint64_t	UInt64;
typedef float		Float;
typedef double		Double;
typedef bool		Bool;
typedef std::string String;

namespace Protocol
{
class StreamWriter;
class StreamReader;
class ProtocolBase
{
public:
	virtual void Initialize() = 0;
	virtual bool Write(StreamWriter& writer) = 0;
	virtual bool Read(StreamReader& writer) = 0;
};

class StreamBase
{
	StreamBase(const StreamBase&) = delete;
	StreamBase& operator=(const StreamBase&) = delete;
public:
	enum Result : int
	{
		OK,
		ReadOverflow,
		WriteOverflow,
	};

public:
	StreamBase() : _writeOffset(0), _readOffset(0), _lastError(0)
	{
		_size = STREAM_DEFAULT_BUFFER_SIZE;
		_buffer = new Int8[_size];
	}

	StreamBase(void* data, size_t size, bool needCopy) : _writeOffset(0), _readOffset(0), _lastError(0)
	{
		_size = size;
		if ( needCopy )
		{
			_buffer = new Int8[_size];
			memcpy_s(_buffer, _size, data, size);
		}
		else
		{
			_buffer = (Int8*)data;
		}
	}

	virtual ~StreamBase() {
		if ( _buffer != nullptr )
		{
			delete[] _buffer;
			_buffer = nullptr;
		}
	}

	inline bool Write(const void* data, size_t index, size_t size)
	{
		if ( _writeOffset < size )
		{
			_lastError = WriteOverflow;
			return false;
		}

		memcpy(_buffer + index, data, size);
		return true;
	}


	const Int8* GetBuffer() { return _buffer; }

	size_t GetWriteOffset() { return _writeOffset; }

	void SetPacketData(void* data, unsigned int size)
	{
		if ( size >= STREAM_DEFAULT_BUFFER_SIZE )
		{
			return;
		}

		_size = size;
		_readOffset = 0;
		_writeOffset = _size;

		memcpy_s(_buffer, _size, data, size);
	}

	void* WriteBuffer() { return _buffer + _writeOffset; }
	void* ReadBuffer() { return _buffer + _readOffset; }

	bool AvaliableWrite(const size_t& size) {
		return _writeOffset + size <= _size ? true : false;
	}

	bool AvaliableRead(const size_t& size) {
		return _readOffset + size <= _writeOffset ? true : false;
	}

	int GetLastError() { return _lastError; }

	UInt32 Peek()
	{
		UInt32 data = 0;
		size_t size = sizeof(UInt32);

		if ( AvaliableRead(size) == false )
		{
			_lastError = ReadOverflow;
			return data;
		}

		memcpy((void*)&data, ReadBuffer(), size);
		return data;
	}

#pragma region Write
	template <typename type>
	inline void operator << (const type& data)
	{
		size_t size = sizeof(type);

		if ( AvaliableWrite(size) == false )
		{
			_lastError = WriteOverflow;
			return;
		}

		memcpy(WriteBuffer(), &data, size);
		_writeOffset += size;
	}

	template <>
	inline void operator << <std::string>( const std::string& data )
	{
		unsigned short stringLength = (short)data.length();

		if ( AvaliableWrite(sizeof(unsigned short) + stringLength) == false )
		{
			_lastError = WriteOverflow;
			return;
		}

		Int8* buffer = (Int8*)WriteBuffer();
		memcpy(buffer, &stringLength, sizeof(unsigned short));
		memcpy(buffer + sizeof(unsigned short), data.c_str(), stringLength);

		_writeOffset += ( sizeof(unsigned short) + stringLength );
	}


	template <typename type>
	inline void operator << (const std::vector<type>& data)
	{
		unsigned short length = (short)data.size();
		if ( AvaliableWrite(sizeof(unsigned short) + sizeof(type) * length) == false )
		{
			_lastError = WriteOverflow;
			return;
		}

		Int8* buffer = (Int8*)WriteBuffer();
		memcpy(buffer, &length, sizeof(unsigned short));
		if ( length > 0 )
		{
			memcpy(buffer + sizeof(unsigned short), &data[0], sizeof(type) * length);
		}
		_writeOffset += ( sizeof(unsigned short) + sizeof(type) * length );
	}

	// Bool 타입만 std::vector에서 raw pointer 접근이 되지 않는다..
	template <>
	inline void operator << <Bool> (const std::vector<Bool>& data)
	{
		unsigned short length = (short)data.size();
		if ( AvaliableWrite(sizeof(unsigned short) + sizeof(Bool) * length) == false )
		{
			_lastError = WriteOverflow;
			return;
		}

		Int8* buffer = (Int8*)WriteBuffer();
		memcpy(buffer, &length, sizeof(unsigned short));
		if ( length > 0 )
		{
			for ( unsigned short i = 0; i < length; ++i )
			{
				Bool temp = data[i];
				memcpy(buffer + sizeof(unsigned short) + sizeof(Bool)* i, &temp, sizeof(Bool) * length);
			}
		}
		_writeOffset += ( sizeof(unsigned short) + sizeof(Bool) * length );
	}

	template <>
	inline void operator << < std::string >( const std::vector<std::string>& data )
	{
		unsigned short length = (short)data.size();
		
		*this >> length;

		if ( _lastError != 0 )
			return;

		for ( unsigned short i = 0; i < length; ++i )
		{
			*this << data[i];
			if ( _lastError != 0 )
				return;
		}
	}
	
#pragma endregion

#pragma region Read
	template <typename type>
	inline void operator >> (const type& data)
	{
		size_t size = sizeof(type);

		if ( AvaliableRead(size) == false )
		{
			_lastError = ReadOverflow;
			return;
		}

		memcpy((void*)&data, ReadBuffer(), size);
		_readOffset += size;
	}

	template <>
	inline void operator >> <std::string>( const std::string& data )
	{
		unsigned short stringLength = 0;
		if ( AvaliableRead(sizeof(unsigned short)) == false )
		{
			_lastError = ReadOverflow;
			return;
		}

		Int8* buffer = (Int8*)ReadBuffer();
		memcpy((void*)&stringLength, buffer, sizeof(unsigned short));

		if ( AvaliableRead(stringLength) == false )
		{
			_lastError = ReadOverflow;
			return;
		}

		( ( std::string* )&data )->resize(stringLength);

		memcpy((void*)data.c_str(), buffer + sizeof(unsigned short), stringLength);
		_readOffset += ( sizeof(unsigned short) + stringLength );
	}
	
	template <typename type>
	inline void operator >> (const std::vector<type>& data)
	{
		unsigned short dataLength = 0;
		
		if ( AvaliableRead(sizeof(unsigned short)) == false )
		{
			_lastError = ReadOverflow;
			return;
		}

		Int8* buffer = (Int8*)ReadBuffer();
		memcpy((void*)&dataLength, buffer, sizeof(unsigned short));
		( ( std::vector<type>* )&data )->resize(dataLength);
		_readOffset += sizeof(unsigned short);

		if ( AvaliableRead(dataLength * sizeof(type)) == false )
		{
			_lastError = ReadOverflow;
			return;
		}

		memcpy((void*)&data[0], buffer + sizeof(unsigned short), dataLength * sizeof(type));
		_readOffset += ( sizeof(type) * dataLength );
	}

	template <>
	inline void operator >> <std::string>(const std::vector<std::string>& data)
	{
		unsigned short dataLength = 0;		
		*this >> dataLength;

		if ( _lastError != 0 )
			return;

		((std::vector<std::string>*)&data)->resize(dataLength);
		_readOffset += sizeof(unsigned short);

		for ( unsigned short i = 0; i < dataLength; ++i )
		{
			*this >> data[i];
			if ( _lastError != 0 )
				return;
		}
	}

	template <>
	inline void operator >> <Bool>( const std::vector<Bool>& data )
	{
		unsigned short dataLength = 0;
		*this >> dataLength;

		if ( _lastError != 0 )
			return;

		( ( std::vector<Bool>* )&data )->resize(dataLength);
		_readOffset += sizeof(unsigned short);

		for ( unsigned short i = 0; i < dataLength; ++i )
		{
			*this >> data[i];
			if ( _lastError != 0 )
				return;
		}
	}
#pragma endregion

protected:
	Int8* _buffer;

	size_t _writeOffset;
	size_t _readOffset;
	size_t _size;

	int _lastError;
};

class StreamWriter
{
public:
	StreamWriter(StreamBase* stream) : _stream(stream) {}

#pragma region ImplementWrite
	template <typename type>
	inline bool Write(type &data)
	{
		if ( std::is_base_of<ProtocolBase, type>::value )
		{
			return data.Write(*this);
		}

		return false;
	}

	// std::vector를 이용한 리스트형 데이터 처리.
	template <typename type>
	inline bool Write(std::vector<type> &data)
	{
		if ( std::is_base_of<ProtocolBase, type>::value )
		{
			size_t length = data.size();
			for ( size_t i = 0; i < length; ++i )
				if ( Write(data[i]) == false )
					return false;
			return true;
		}
		return false;
	}

	inline bool Write(const void* data, size_t index, size_t size)
	{
		_stream->Write(data, index, size);
		return _stream->GetLastError() == 0;
	}

#define DEFAULT_WRITE_TYPE_TEMPLATE(type) \
	template <> \
	inline bool Write<type>(type &item) \
	{ \
		*_stream << item; \
		return _stream->GetLastError() == 0; \
	} \
	template <> \
	inline bool Write<std::vector<type>>(std::vector<type> &item) \
	{ \
		*_stream << item; \
		return _stream->GetLastError() == 0; \
	}

	inline bool Write(const UInt32 &item)
	{
		*_stream << item;
		return _stream->GetLastError() == 0;
	}

	DEFAULT_WRITE_TYPE_TEMPLATE(Int8);
	DEFAULT_WRITE_TYPE_TEMPLATE(Int16);
	DEFAULT_WRITE_TYPE_TEMPLATE(Int32);
	DEFAULT_WRITE_TYPE_TEMPLATE(Int64);
	DEFAULT_WRITE_TYPE_TEMPLATE(UInt32);
	DEFAULT_WRITE_TYPE_TEMPLATE(UInt16);
	DEFAULT_WRITE_TYPE_TEMPLATE(UInt8);
	DEFAULT_WRITE_TYPE_TEMPLATE(UInt64);
	DEFAULT_WRITE_TYPE_TEMPLATE(Float);
	DEFAULT_WRITE_TYPE_TEMPLATE(Double);
	DEFAULT_WRITE_TYPE_TEMPLATE(Bool);
	DEFAULT_WRITE_TYPE_TEMPLATE(String);

#pragma endregion
#pragma region Function
	inline size_t GetSize()
	{
		return _stream->GetWriteOffset();
	}
#pragma endregion

protected:
	StreamBase *_stream;
};

class StreamReader
{
public:
	StreamReader(StreamBase* stream) : _stream(stream) {}

#pragma region ImplementRead
	template <typename type>
	inline bool Read(type &data)
	{
		if ( std::is_base_of<ProtocolBase, type>::value )
		{
			return data.Read(*this);
		}

		return false;
	}

	template <typename type>
	inline bool Read(std::vector<type> &data)
	{
		if ( std::is_base_of<ProtocolBase, type>::value )
		{
			unsigned short length = 0;
			if ( Read(length) )
			{
				data.resize(length);
				for ( unsigned short i = 0; i < length; ++i )
					if ( Read(data[i]) == false )
						return false;
				return true;
			}
			return true;
		}

		return false;
	}

	UInt32 Peek()
	{
		return _stream->Peek();
	}

#define DEFAULT_READ_TYPE_TEMPLATE(type) \
	template <> \
	inline bool Read<type>(type &item) \
	{ \
		*_stream >> item; \
		return _stream->GetLastError() == 0; \
	} \
	template <> \
	inline bool Read<std::vector<type>>(std::vector<type> &item) \
	{ \
		*_stream >> item; \
		return _stream->GetLastError() == 0; \
	} 

	inline bool Read(const UInt32 &item)
	{
		*_stream >> item;
		return _stream->GetLastError() == 0;
	}

	DEFAULT_READ_TYPE_TEMPLATE(Int8);
	DEFAULT_READ_TYPE_TEMPLATE(Int16);
	DEFAULT_READ_TYPE_TEMPLATE(Int32);
	DEFAULT_READ_TYPE_TEMPLATE(Int64);
	DEFAULT_READ_TYPE_TEMPLATE(UInt32);
	DEFAULT_READ_TYPE_TEMPLATE(UInt16);
	DEFAULT_READ_TYPE_TEMPLATE(UInt8);
	DEFAULT_READ_TYPE_TEMPLATE(UInt64);
	DEFAULT_READ_TYPE_TEMPLATE(Float);
	DEFAULT_READ_TYPE_TEMPLATE(Double);
	DEFAULT_READ_TYPE_TEMPLATE(Bool);
	DEFAULT_READ_TYPE_TEMPLATE(String);
#pragma endregion
protected:
	StreamBase *_stream;
};


class ProtocolConstructor
{
protected:
	ProtocolConstructor(UInt32 protocolID) : _protocolID(protocolID) {}
public:
	virtual ProtocolBase* CreateProtocol() = 0;

	UInt32 GetProtocolID() { return _protocolID; }
private:
	UInt32 _protocolID;
};

template <typename ProtocolType>
class GenericProtocolConstructor : public ProtocolConstructor
{
public:
	GenericProtocolConstructor() : ProtocolConstructor(ProtocolType::__ID__)
	{
	}

	virtual ProtocolBase* CreateProtocol()
	{
		return new ProtocolType;
	}
};

typedef std::shared_ptr<Protocol::ProtocolBase> ProtocolSharedPtr;
typedef std::shared_ptr<Protocol::ProtocolConstructor> ProtocolConstructorSharedPtr;
}