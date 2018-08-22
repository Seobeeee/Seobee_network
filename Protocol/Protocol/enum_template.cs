<%page enable_loop="True"/>
public enum ${ class_name }
{
    #region Parameter
    % for member in class_data.member_list:
    ${member.name} = ${member.value},
    % endfor
};
