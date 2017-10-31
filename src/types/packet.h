
#ifndef STARFLOW_ANALYTICS_PACKET_H
#define STARFLOW_ANALYTICS_PACKET_H

#include <chrono>
#include <string>

namespace starflow {
	namespace types {
		class Packet
		{
		public:
			//TODO:	proto::packet to_proto() const;
			//TODO:	static Packet from_proto(const proto::packet);

			Packet()                         = default;
			Packet(std::chrono::microseconds ts, unsigned int len);
			Packet(unsigned long ts, unsigned int len);
			Packet(const Packet&)            = default;
			Packet& operator=(const Packet&) = default;
			Packet(Packet&&)                 = default;
			Packet& operator=(Packet&&)      = default;

			std::chrono::microseconds ts;
			unsigned                  len;

			std::string str_desc() const;

			virtual ~Packet();
		};
	}
}

#endif
