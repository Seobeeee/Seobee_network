#pragma once

#ifndef PACKET_PROTOCOL_VERSION
#define PACKET_PROTOCOL_VERSION 15
#endif

#include "Protocol.h"

namespace Protocol
{
		
enum RoomType : Int16
{
	NormalGameRoom = 1,
	CommunityRoom = 2,
};

		
class UserInfoTest : public Protocol::ProtocolBase
{
	PROTOCOL_CLASS_DEF_WITHOUT_REGISTER(UserInfoTest)
public:
	Int32 listID;
	Int64 playerCSN;
	Int64 playerGSN;
	String nickName;
	String publicIP;

public:
    virtual void Initialize()	
    {
		listID = 0;
		playerCSN = 0;
		playerGSN = 0;
    }
	
	virtual inline bool Write(Protocol::StreamWriter& writer)
	{
		bool result = true;
		do 
		{
			result &= writer.Write(listID);
			if ( !result ) break;
			result &= writer.Write(playerCSN);
			if ( !result ) break;
			result &= writer.Write(playerGSN);
			if ( !result ) break;
			result &= writer.Write(nickName);
			if ( !result ) break;
			result &= writer.Write(publicIP);
			if ( !result ) break;
		} while (0);
		return result;
	}

	virtual inline bool Read(Protocol::StreamReader& reader)
	{
		bool result = true;
		do
		{
            result &= reader.Read(listID);
			if ( !result ) break;
            result &= reader.Read(playerCSN);
			if ( !result ) break;
            result &= reader.Read(playerGSN);
			if ( !result ) break;
            result &= reader.Read(nickName);
			if ( !result ) break;
            result &= reader.Read(publicIP);
			if ( !result ) break;
		} while ( 0 );
			return result;
	}
};

		
class CharacterEnterMatchingRequest : public Protocol::ProtocolBase
{
	PROTOCOL_CLASS_DEF(CharacterEnterMatchingRequest)
public:
	RoomType roomtype;

public:
	static const UInt32 __ID__ = 2106486250LL;

    virtual void Initialize()
    {
    }
	
	virtual inline bool Write(Protocol::StreamWriter& writer)
	{
		bool result = true;
		do 
		{
			result &= writer.Write(__ID__);
			if ( !result ) break;
			result &= writer.Write((Int16)roomtype);
			if ( !result ) break;
		} while (0);
		return result;
	}

	virtual inline bool Read(Protocol::StreamReader& reader)
	{
		bool result = true;
		do
		{
			UInt32 id = 0;
			result &= reader.Read(id);
			if ( !result || id != __ID__ ) break;
			result &= reader.Read((Int16)roomtype);
			if ( !result ) break;
		} while ( 0 );
			return result;
	}
};

		
class CharacterEnterMatchingRequestReply : public Protocol::ProtocolBase
{
	PROTOCOL_CLASS_DEF(CharacterEnterMatchingRequestReply)
public:
	Int8 result;
	Int8 playerCount;
	Int32 listID;
	std::vector<UserInfoTest> userInfoTest;

public:
	static const UInt32 __ID__ = 1961385647LL;

    virtual void Initialize()
    {
		result = 0;
		playerCount = 0;
		listID = 0;
    }
	
	virtual inline bool Write(Protocol::StreamWriter& writer)
	{
		bool result = true;
		do 
		{
			result &= writer.Write(__ID__);
			if ( !result ) break;
			result &= writer.Write(result);
			if ( !result ) break;
			result &= writer.Write(playerCount);
			if ( !result ) break;
			result &= writer.Write(listID);
			if ( !result ) break;
			result &= writer.Write(userInfoTest);
			if ( !result ) break;
		} while (0);
		return result;
	}

	virtual inline bool Read(Protocol::StreamReader& reader)
	{
		bool result = true;
		do
		{
			UInt32 id = 0;
			result &= reader.Read(id);
			if ( !result || id != __ID__ ) break;
            result &= reader.Read(result);
			if ( !result ) break;
            result &= reader.Read(playerCount);
			if ( !result ) break;
            result &= reader.Read(listID);
			if ( !result ) break;
            result &= reader.Read(userInfoTest);
			if ( !result ) break;
		} while ( 0 );
			return result;
	}
};

		
class ReplyNotifyCharacterEnterRoom : public Protocol::ProtocolBase
{
	PROTOCOL_CLASS_DEF(ReplyNotifyCharacterEnterRoom)
public:
	Int8 result;
	Int8 playerCount;
	Int32 listID;
	Int64 playerCSN;
	Int64 playerGSN;
	String nickName;
	String publicIP;

public:
	static const UInt32 __ID__ = 339022714LL;

    virtual void Initialize()
    {
		result = 0;
		playerCount = 0;
		listID = 0;
		playerCSN = 0;
		playerGSN = 0;
    }
	
	virtual inline bool Write(Protocol::StreamWriter& writer)
	{
		bool result = true;
		do 
		{
			result &= writer.Write(__ID__);
			if ( !result ) break;
			result &= writer.Write(result);
			if ( !result ) break;
			result &= writer.Write(playerCount);
			if ( !result ) break;
			result &= writer.Write(listID);
			if ( !result ) break;
			result &= writer.Write(playerCSN);
			if ( !result ) break;
			result &= writer.Write(playerGSN);
			if ( !result ) break;
			result &= writer.Write(nickName);
			if ( !result ) break;
			result &= writer.Write(publicIP);
			if ( !result ) break;
		} while (0);
		return result;
	}

	virtual inline bool Read(Protocol::StreamReader& reader)
	{
		bool result = true;
		do
		{
			UInt32 id = 0;
			result &= reader.Read(id);
			if ( !result || id != __ID__ ) break;
            result &= reader.Read(result);
			if ( !result ) break;
            result &= reader.Read(playerCount);
			if ( !result ) break;
            result &= reader.Read(listID);
			if ( !result ) break;
            result &= reader.Read(playerCSN);
			if ( !result ) break;
            result &= reader.Read(playerGSN);
			if ( !result ) break;
            result &= reader.Read(nickName);
			if ( !result ) break;
            result &= reader.Read(publicIP);
			if ( !result ) break;
		} while ( 0 );
			return result;
	}
};

}

