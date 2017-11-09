
#include "live_capture.h"
#include "../types/raw_packet.h"

starflow::kernels::LiveCapture::LiveCapture(const std::string& dev, int snaplen, int to_ms)
	: raft::kernel(), _pcap(nullptr)
{
	_connect(dev, snaplen, to_ms);
	output.add_port<types::RawPacket>("out");
}

raft::kstatus starflow::kernels::LiveCapture::run()
{
	struct pcap_pkthdr* hdr;
	const u_char* pl;
	int rc = -1;

	while ((rc = pcap_next_ex(_pcap, &hdr, &pl)) != 1);

	output["out"].push(types::RawPacket(hdr, pl));
	return(raft::proceed);
}

void starflow::kernels::LiveCapture::_connect(const std::string& dev, int snaplen, int to_ms)
	throw(std::runtime_error)
{
	char errbuf[PCAP_ERRBUF_SIZE];

	_pcap = pcap_open_live(dev.c_str(), snaplen, 0, to_ms, errbuf);

	if(_pcap == nullptr) {
		std::string msg = "PacketCapture::_connect: could not connect to " + dev + ": " + errbuf;
		throw std::runtime_error(msg);
	}
}
