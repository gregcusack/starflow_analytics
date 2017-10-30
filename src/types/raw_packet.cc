
#include "raw_packet.h"

starflow::types::RawPacket::RawPacket(unsigned long ts, unsigned len, const unsigned char* pl)
	: ts(ts), len(len), pl(pl)
{ }

starflow::types::RawPacket::RawPacket(struct pcap_pkthdr* hdr, const u_char* pl)
	: ts((unsigned) hdr->ts.tv_sec * 1000000 + (unsigned) hdr->ts.tv_usec), len(hdr->len), pl(pl)
{ }
