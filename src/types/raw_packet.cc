
#include "raw_packet.h"

starflow::types::RawPacket::RawPacket(const starflow::types::RawPacket& copy_from)
	: ts(copy_from.ts), len(copy_from.len), pl(new unsigned char[len])
{
	std::memcpy(this->pl.get(), pl.get(), len);
}

starflow::types::RawPacket& starflow::types::RawPacket::operator=(const starflow::types::RawPacket& copy_from)
{
	this->pl = std::unique_ptr<unsigned char[]>(new unsigned char[len]);
	this->ts  = copy_from.ts;
	this->len = copy_from.len;
	std::memcpy(this->pl.get(), copy_from.pl.get(), len);
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
