
#include <sys/socket.h>
#include <netinet/in.h>

#include "../types/raw_packet.h"

#include "tzsp_receiver.h"

starflow::kernels::TZSPReceiver::TZSPReceiver(unsigned short listen_port)
	: _tzsp_receiver(listen_port, [this](types::RawPacket p){ output["out"].push(p); })
{
	output.add_port<types::RawPacket>("out");
}

raft::kstatus starflow::kernels::TZSPReceiver::run()
{
	_tzsp_receiver();
	return raft::proceed;
}
