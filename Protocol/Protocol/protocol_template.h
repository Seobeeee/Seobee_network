#pragma once
<%page enable_loop = "True"/>
#ifndef PROTOCOL_VERSION
#define PROTOCOL_VERSION ${ version }
#endif

#include "Protocol.h"

namespace Protocol
{
	% for packet in packet_list :
		${ packet.render(packet.name) }
	% endfor
}

