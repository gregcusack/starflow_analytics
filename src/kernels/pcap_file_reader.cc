
#include "pcap_file_reader.h"

#include "../types/raw_packet.h"

starflow::kernels::PCAPFileReader::PCAPFileReader(const std::string& file_name)
	: _pcap_reader()
{
	_pcap_reader.set_file_name(file_name);
	_pcap_reader.set_callback([this](types::RawPacket p) {
		this->_read_packet(std::move(p));
	});
	output.add_port<starflow::types::RawPacket>("out");
}

raft::kstatus starflow::kernels::PCAPFileReader::run()
{
	/*
	struct pcap_pkthdr* hdr;
	const u_char* pl;

	pcap_next_ex(_pcap, &hdr, &pl);
	//std::cout << &pl << std::endl;
	output["out"].push(types::RawPacket(hdr, pl));
	return(raft::proceed);
	*/
	_pcap_reader();
	return(raft::stop);
}

void starflow::kernels::PCAPFileReader::_read_packet(starflow::types::RawPacket p)
{
	output["out"].push(p);
}
