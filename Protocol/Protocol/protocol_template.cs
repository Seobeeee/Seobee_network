<%page enable_loop = "True"/>
namespace Protocol
{
	% for packet in packet_list :
		${ packet.render(packet.name) }
	% endfor
}

