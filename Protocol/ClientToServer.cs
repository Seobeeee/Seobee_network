
namespace Protocol
{
		
    public class CharacterEnterMatchingRequest : ProtocolBase
    {
	    #region Parameter	
	    public const uint ID = 2106486250;
	    public sbyte roomtype;
        #endregion

        public static string ProtocolName
        {
		    get 
		    {
			    return "CharacterEnterMatchingRequest";
            }
        }

        public CharacterEnterMatchingRequest()
	    { 

            Initialize(); 
	    }

        public void Initialize()
        {

		    roomtype = 0;

        }

        public  bool Write(Protocol.StreamWriter writer)
	    {
		    bool result = true;
		    do 
		    {
			    result &= writer.Write(ID);
			    if ( !result ) break;
			    result &= writer.Write(roomtype);
			    if ( !result ) break;
		    } while (false);
		    return result;
	    }

	    public bool Read(Protocol.StreamReader reader)
	    {
		    bool result = true;
		    do
		    {
                uint id = 0;
			    result &= reader.Read(ref id);
			    if ( !result || id != ID ) break;
			    result &= reader.Read(ref roomtype);
			    if ( !result ) break;
		    } while ( false );
		    return result;
	    }
};

		
    public class CharacterEnterMatchingRequestReply : ProtocolBase
    {
	    #region Parameter	
	    public const uint ID = 1961385647;
	    public sbyte result;
	    public sbyte playerCount;
	    public int listID;
	    public long playerCSN;
	    public long playerGSN;
	    public string nickName;
	    public string publicIP;
        #endregion

        public static string ProtocolName
        {
		    get 
		    {
			    return "CharacterEnterMatchingRequestReply";
            }
        }

        public CharacterEnterMatchingRequestReply()
	    { 

            Initialize(); 
	    }

        public void Initialize()
        {

		    result = 0;


		    playerCount = 0;


		    listID = 0;


		    playerCSN = 0;


		    playerGSN = 0;



        }

        public  bool Write(Protocol.StreamWriter writer)
	    {
		    bool result = true;
		    do 
		    {
			    result &= writer.Write(ID);
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
		    } while (false);
		    return result;
	    }

	    public bool Read(Protocol.StreamReader reader)
	    {
		    bool result = true;
		    do
		    {
                uint id = 0;
			    result &= reader.Read(ref id);
			    if ( !result || id != ID ) break;
			    result &= reader.Read(ref result);
			    if ( !result ) break;
			    result &= reader.Read(ref playerCount);
			    if ( !result ) break;
			    result &= reader.Read(ref listID);
			    if ( !result ) break;
			    result &= reader.Read(ref playerCSN);
			    if ( !result ) break;
			    result &= reader.Read(ref playerGSN);
			    if ( !result ) break;
			    result &= reader.Read(ref nickName);
			    if ( !result ) break;
			    result &= reader.Read(ref publicIP);
			    if ( !result ) break;
		    } while ( false );
		    return result;
	    }
};

		
    public class ReplyNotifyCharacterEnterRoom : ProtocolBase
    {
	    #region Parameter	
	    public const uint ID = 339022714;
	    public sbyte result;
	    public sbyte playerCount;
	    public int listID;
	    public long playerCSN;
	    public long playerGSN;
	    public string nickName;
	    public string publicIP;
        #endregion

        public static string ProtocolName
        {
		    get 
		    {
			    return "ReplyNotifyCharacterEnterRoom";
            }
        }

        public ReplyNotifyCharacterEnterRoom()
	    { 

            Initialize(); 
	    }

        public void Initialize()
        {

		    result = 0;


		    playerCount = 0;


		    listID = 0;


		    playerCSN = 0;


		    playerGSN = 0;



        }

        public  bool Write(Protocol.StreamWriter writer)
	    {
		    bool result = true;
		    do 
		    {
			    result &= writer.Write(ID);
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
		    } while (false);
		    return result;
	    }

	    public bool Read(Protocol.StreamReader reader)
	    {
		    bool result = true;
		    do
		    {
                uint id = 0;
			    result &= reader.Read(ref id);
			    if ( !result || id != ID ) break;
			    result &= reader.Read(ref result);
			    if ( !result ) break;
			    result &= reader.Read(ref playerCount);
			    if ( !result ) break;
			    result &= reader.Read(ref listID);
			    if ( !result ) break;
			    result &= reader.Read(ref playerCSN);
			    if ( !result ) break;
			    result &= reader.Read(ref playerGSN);
			    if ( !result ) break;
			    result &= reader.Read(ref nickName);
			    if ( !result ) break;
			    result &= reader.Read(ref publicIP);
			    if ( !result ) break;
		    } while ( false );
		    return result;
	    }
};

}

