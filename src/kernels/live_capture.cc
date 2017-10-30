
#include "live_capture.h"
#include "../types/raw_packet.h"

starflow::kernels::LiveCapture::LiveCapture(const std::string& dev)
	: raft::kernel(), _pcap(nullptr)
{
	_connect(dev);
	output.add_port<types::RawPacket>("out");
}

raft::kstatus starflow::kernels::LiveCapture::run()
{
	struct pcap_pkthdr* hdr;
	const u_char* pl;

	pcap_next_ex(_pcap, &hdr, &pl);
	output["out"].push(types::RawPacket(hdr, pl));
	return(raft::proceed);
}

void starflow::kernels::LiveCapture::_connect(const std::string& dev) throw(std::runtime_error)
{
	char errbuf[PCAP_ERRBUF_SIZE];

	_pcap = pcap_open_live(dev.c_str(), 1600, 0, 500, errbuf);

	if(_pcap == nullptr) {
		std::string msg = "PacketCapture::_connect: could not connect to " + dev + ": " + errbuf;
		throw std::runtime_error(msg);
	}
}
