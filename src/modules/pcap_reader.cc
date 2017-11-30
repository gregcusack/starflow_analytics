
#include "pcap_reader.h"

starflow::modules::PCAPReader::PCAPReader(const std::string& file_name, enum PCAPReader::mode mode)
	throw(std::runtime_error)
	: _mode(mode)
{
	_open(file_name);
}

void starflow::modules::PCAPReader::_open(const std::string& file_name)
throw(std::runtime_error)
{
	char errbuf[PCAP_ERRBUF_SIZE];

	if ((_pcap = pcap_open_offline(file_name.c_str(), errbuf)) == nullptr) {
		std::string msg = std::string("PCAPReader: could not open file: ") + errbuf;
		throw std::runtime_error(msg);
	}
}

starflow::modules::PCAPReader::~PCAPReader()
{
	pcap_close(_pcap);
}
