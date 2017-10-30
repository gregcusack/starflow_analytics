
#ifndef STARFLOW_ANALYTICS_RAW_PACKET_H
#define STARFLOW_ANALYTICS_RAW_PACKET_H

#include <ostream>

#include <pcap.h>

namespace starflow {

	class RawPacket
	{
	public:
		RawPacket() = default;

		RawPacket(const RawPacket&) = default;
		RawPacket(RawPacket&&) = default;

		RawPacket& operator=(const RawPacket&) = default;
		RawPacket& operator=(RawPacket&&) = default;


		RawPacket(unsigned long ts, unsigned len, const unsigned char* pl);
		RawPacket(struct pcap_pkthdr* hdr, const u_char* pl);

		unsigned long ts;
		unsigned len;
		const unsigned char* pl;
	};

}

#endif
