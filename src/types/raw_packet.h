
#ifndef STARFLOW_ANALYTICS_RAW_PACKET_H
#define STARFLOW_ANALYTICS_RAW_PACKET_H

#include <memory>
#include <chrono>
#include <pcap.h>

namespace starflow {
	namespace types {
		class RawPacket
		{
		public:
			RawPacket() = default;

			RawPacket(const RawPacket&);
			RawPacket(RawPacket&&) = default;

			RawPacket& operator=(const RawPacket&);
			RawPacket& operator=(RawPacket&&) = default;

			RawPacket(unsigned long ts, unsigned len, const unsigned char* pl);
			RawPacket(struct pcap_pkthdr* hdr, const u_char* pl);

			std::chrono::microseconds ts;
			unsigned len;
			std::unique_ptr<unsigned char[]> pl = nullptr;
		};
	}
}

#endif
