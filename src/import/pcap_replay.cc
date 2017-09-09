
#include "pcap_replay.h"

#include <thread>

starflow::import::PCAPReplay::PCAPReplay(const std::string& file_name) throw(std::runtime_error)
{
	char errbuf[PCAP_ERRBUF_SIZE];

	if ((_pcap = pcap_open_offline(file_name.c_str(), errbuf)) == nullptr)
		throw std::runtime_error(std::string("PCAPReplay: could not open file: ") + errbuf);
}

void starflow::import::PCAPReplay::operator()(
	std::function<void (struct pcap_pkthdr*, const unsigned char*)> f)
{
	struct pcap_pkthdr* header   = nullptr;
	const u_char* packet         = nullptr;

	while (pcap_next_ex(_pcap, &header, &packet) >= 0) {
		auto ts_us = us_from_timeval(header->ts);

		if (_i > 0) {
			auto d = std::chrono::duration_cast<std::chrono::microseconds>(ts_us - _last_ts);
			std::this_thread::sleep_for(d);
		}

		_last_ts = ts_us;
		f(header, packet);
		_i++;
	}
}

starflow::import::PCAPReplay::~PCAPReplay()
{
	pcap_close(_pcap);
}

std::chrono::microseconds starflow::import::PCAPReplay::us_from_timeval(timeval& t)
{
	return std::chrono::microseconds {t.tv_sec * 1000000 + t.tv_usec};
}
