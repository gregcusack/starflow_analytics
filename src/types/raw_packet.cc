
#include "raw_packet.h"

starflow::types::RawPacket::RawPacket(const starflow::types::RawPacket& copy_from)
	: ts(copy_from.ts), len(copy_from.len), pl(nullptr)
{
	auto* from = copy_from.pl.get();
	auto* to = new unsigned char[len];
	std::memcpy(to, from, len);
	pl.reset(to);
}

starflow::types::RawPacket& starflow::types::RawPacket::operator=(const starflow::types::RawPacket& copy_from)
{
	this->ts = copy_from.ts;
	this->len = copy_from.len;

	auto* from = copy_from.pl.get();
	auto* to = new unsigned char[len];
	std::memcpy(to, from, len);
	pl.reset(to);

	return *this;
}

starflow::types::RawPacket::RawPacket(unsigned long ts, unsigned len, const unsigned char* pl)
	: ts(ts), len(len), pl(new unsigned char[len])
{
	std::memcpy(this->pl.get(), pl, len);
}

starflow::types::RawPacket::RawPacket(struct pcap_pkthdr* hdr, const u_char* pl)
	: ts(hdr->ts.tv_sec * 1000000 + hdr->ts.tv_usec), len(hdr->len), pl(new unsigned char[len])
{
	std::memcpy(this->pl.get(), pl, len);
}
