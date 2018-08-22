<%page enable_loop="True"/>
class ${ class_name } : public Protocol::ProtocolBase
{
	PROTOCOL_CLASS_DEF_WITHOUT_REGISTER(${class_name})
public:
% for member in class_data.member_list :
	%if member.type.startswith("List") :
	std::vector<${ member.type[5:] }> ${ member.name };
	% else:
	${member.type} ${member.name};
	% endif
% endfor

public:
    virtual void Initialize()
    {
    % for member in class_data.member_list:
		% if "Int8" == member.type:
		${member.name} = 0;
		% elif "Int16" == member.type:
		${member.name} = 0;
		% elif "Int32" == member.type:
		${member.name} = 0;
		% elif "UInt8" == member.type:
		${member.name} = 0;
		% elif "UInt16" == member.type:
		${member.name} = 0;
		% elif "UInt32" == member.type:
		${member.name} = 0;
		% elif "Int64" == member.type:
		${member.name} = 0;
		% elif "UInt64" == member.type:
		${member.name} = 0;
		% elif "Float" == member.type:
		${member.name} = 0.0f;
		% elif "Double" == member.type:
		${member.name} = 0.0;
		% elif "Bool" == member.type:
		${member.name} = false;
		% endif
	% endfor
    }
	
	virtual inline bool Write(Protocol::StreamWriter& writer)
	{
		bool result = true;
		do 
		{
			% for member in class_data.member_list:
			% if member.is_enum:
			result &= writer.Write((Int16)${ member.name });
			% else:
			result &= writer.Write(${ member.name });
			% endif
			if ( !result ) break;
			% endfor
		} while (0);
		return result;
	}

	virtual inline bool Read(Protocol::StreamReader& reader)
	{
		bool result = true;
		do
		{
			% for member in class_data.member_list:
			% if member.is_enum:
			result &= reader.Read((Int16)${ member.name });
			% else:
            result &= reader.Read(${ member.name });
			% endif
			if ( !result ) break;
			% endfor
		} while ( 0 );
			return result;
	}
};
