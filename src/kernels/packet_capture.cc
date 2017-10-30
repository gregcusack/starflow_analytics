
#include "packet_capture.h"
#include "../proto/starflow.pb.h"

starflow::kernels::PacketCapture::PacketCapture(const std::string& dev)
	: raft::kernel(), _pcap(nullptr)
{
	_connect(dev);
	output.add_port<proto::raw_packet>("out");
}

raft::kstatus starflow::kernels::PacketCapture::run()
{
	struct pcap_pkthdr* hdr;
	const u_char* pl;

	pcap_next_ex(_pcap, &hdr, &pl);

	int len = hdr->len;
	long usec = hdr->ts.tv_sec * 1000000 + hdr->ts.tv_usec;

	proto::raw_packet p;
	p.set_ts(usec);
	p.set_len(len);
	p.set_pl(reinterpret_cast<const char*>(pl));

	auto out(output["out"].template allocate_s<proto::raw_packet>());
	*out = p;

	return(raft::proceed);
}

void starflow::kernels::PacketCapture::_connect(const std::string& dev) throw(std::runtime_error)
{
	char errbuf[PCAP_ERRBUF_SIZE];

	_pcap = pcap_open_live(dev.c_str(), 1600, 0, 500, errbuf);

	if(_pcap == nullptr) {
		std::string msg = "PacketCapture::_connect: could not connect to " + dev + ": " + errbuf;
		throw std::runtime_error(msg);
	}
}
