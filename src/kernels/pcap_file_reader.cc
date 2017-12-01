
#include "pcap_file_reader.h"

#include "../types/raw_packet.h"
/*
starflow::kernels::PCAPFileReader::PCAPFileReader(const std::string& file_name)
{
	_open(file_name);
	output.add_port<types::RawPacket>("out");
}

raft::kstatus starflow::kernels::PCAPFileReader::run()
{
	struct pcap_pkthdr* hdr;
	const u_char* pl;

	pcap_next_ex(_pcap, &hdr, &pl);
	output["out"].push(types::RawPacket(hdr, pl));
	return(raft::proceed);
}

void starflow::kernels::PCAPFileReader::_open(const std::string& file_name)
	throw(std::runtime_error)
{
	char errbuf[PCAP_ERRBUF_SIZE];

	if ((_pcap = pcap_open_offline(file_name.c_str(), errbuf)) == nullptr) {
		std::string msg = std::string("PCAPFileReader: could not open file: ") + errbuf;
		throw std::runtime_error(msg);
	}
}
*/