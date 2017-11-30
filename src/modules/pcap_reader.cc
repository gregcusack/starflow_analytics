
#include "pcap_reader.h"

void starflow::modules::PCAPReader::set_file_name(const std::string& file_name)
{
	_file_name = file_name;
}

void starflow::modules::PCAPReader::set_mode(mode mode)
{
	_mode = mode;
}

void starflow::modules::PCAPReader::set_store_mode()
{
	_mode = mode::store;
}

void starflow::modules::PCAPReader::set_emit_mode()
{
	_mode = mode::emit;
}

void starflow::modules::PCAPReader::set_callback(std::function<void (types::RawPacket)> callback)
{
	_callback = std::move(callback);
}
/*
void starflow::modules::PCAPReader::set_playback(bool playback)
{
	_playback = playback;
}
*/
const starflow::modules::PCAPReader& starflow::modules::PCAPReader::operator()()
	throw(std::runtime_error, std::logic_error)
{
	struct pcap_pkthdr* hdr;
	const u_char* pl;

	if (_file_name.empty())
		throw std::logic_error("PCAPReader: no file name set");

	if (_mode == mode::emit && _callback == nullptr)
		throw std::logic_error("PCAPReader: no callback set");

	_open(_file_name);

	for (;;) {
		int status = pcap_next_ex(_pcap, &hdr, &pl);
		if (status == 1) {
			if (_mode == mode::emit)
				_callback(types::RawPacket(hdr, pl));
			else
				_packets.emplace_back(hdr, pl);
			continue;
		} else if (status == -2) {
			break;
		} else {
			_close();
			throw std::runtime_error(std::string("PCAPReader: error reading pcap file")
									 + pcap_geterr(_pcap));
		}
	}

	_close();

	return *this;
}

const std::list<starflow::types::RawPacket>& starflow::modules::PCAPReader::packets() const
{
	return _packets;
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

void starflow::modules::PCAPReader::_close()
{
	pcap_close(_pcap);
}
