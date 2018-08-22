<%page enable_loop="True"/>
enum ${ class_name } : Int16
{
	% for member in class_data.member_list :
	${ member.name } = ${member.value},
	% endfor
};
