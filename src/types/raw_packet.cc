
#include "raw_packet.h"

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