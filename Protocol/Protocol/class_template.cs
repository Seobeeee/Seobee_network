<%page enable_loop="True"/>
    public class ${ class_name } : ProtocolBase
    {
	    #region Parameter	
	    public const uint ID = ${class_data.id};
        % for member in class_data.member_list:
	    % if "Int8" == member.type:
	    public sbyte ${member.name};
	    % elif "Int16" == member.type:
	    public short ${member.name};
	    % elif "Int32" == member.type:
	    public int ${member.name};
	    % elif "UInt8" == member.type:
	    public byte ${member.name};
	    % elif "UInt16" == member.type:
	    public ushort ${member.name};
	    % elif "UInt32" == member.type:
	    public uint ${member.name};
	    % elif "Int64" == member.type:
	    public long ${member.name};
	    % elif "UInt64" == member.type:
	    public ulong ${member.name};
	    % elif "Float" == member.type:
	    public float ${member.name};
	    % elif "Double" == member.type:
	    public double ${member.name};
	    % elif "Bool" == member.type:
	    public bool ${member.name};
	    % elif "String" == member.type:
	    public string ${member.name};
	    % endif
    % endfor
        #endregion

        public static string ProtocolName
        {
		    get 
		    {
			    return "${ class_name }";
            }
        }

        public ${ class_name }()
	    { 

            Initialize(); 
	    }

        public void Initialize()
        {
        % for member in class_data.member_list:

            % if "Int8" == member.type:
		    ${ member.name} = 0;

            % elif "Int16" == member.type:
		    ${ member.name} = 0;

            % elif "Int32" == member.type:
		    ${ member.name} = 0;

            % elif "UInt8" == member.type:
		    ${ member.name} = 0;

            % elif "UInt16" == member.type:
		    ${ member.name} = 0;

            % elif "UInt32" == member.type:
		    ${ member.name} = 0;

            % elif "Int64" == member.type:
		    ${ member.name} = 0;

            % elif "UInt64" == member.type:
		    ${ member.name} = 0;

            % elif "Float" == member.type:
		    ${ member.name} = 0.0f;

            % elif "Double" == member.type:
		    ${ member.name} = 0.0;

            % elif "Bool" == member.type:
		    ${ member.name} = false;

            % endif
        % endfor
        }

        public  bool Write(Protocol.StreamWriter writer)
	    {
		    bool result = true;
		    do 
		    {
			    result &= writer.Write(ID);
			    if ( !result ) break;
			    % for member in class_data.member_list:
			    result &= writer.Write(${ member.name });
			    if ( !result ) break;
			    % endfor
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
			    % for member in class_data.member_list:
			    result &= reader.Read(ref ${ member.name });
			    if ( !result ) break;
			    % endfor
		    } while ( false );
		    return result;
	    }
};
