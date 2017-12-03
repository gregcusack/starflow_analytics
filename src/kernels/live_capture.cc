
#include "live_capture.h"
#include "../types/raw_packet.h"

starflow::kernels::LiveCapture::LiveCapture(const std::string& dev)
	: raft::kernel(),
	  _live_capture(dev, [this](types::RawPacket p) { this->output["out"].push(p); })
{
	output.add_port<types::RawPacket>("out");
}

raft::kstatus starflow::kernels::LiveCapture::run()
{
	_live_capture();
	return(raft::proceed);
}
