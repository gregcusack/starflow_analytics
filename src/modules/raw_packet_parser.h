
#ifndef STARFLOW_ANALYTICS_MODULES_RAW_PACKET_PARSER_H
#define STARFLOW_ANALYTICS_MODULES_RAW_PACKET_PARSER_H

#include "../types/raw_packet.h"
#include "../types/key.h"
#include "../types/packet.h"

namespace starflow {
	namespace modules {

		class RawPacketParser
		{
		public:
			enum class outer_header_type { eth, ip };
			enum class capture_length { full, trunc };

			explicit RawPacketParser(outer_header_type t = outer_header_type::eth,
									 capture_length l = capture_length::full);

			void set_outer_header_type(outer_header_type t);
			void set_capture_length(capture_length l);

			std::pair<types::Key, types::Packet> operator()(const types::RawPacket& raw_packet);

			void operator()(const types::RawPacket& raw_packet, types::Key& key,
							types::Packet& packet);

		private:
			outer_header_type _outer_header;
			capture_length _capture_length;
			bool _parse_packet(const types::RawPacket& raw_packet, types::Key& key,
							   types::Packet& packet);
		};
	}
}

#endif
