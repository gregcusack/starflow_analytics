
#ifndef STARFLOW_ANALYTICS_RAW_PACKET_PARSER_H
#define STARFLOW_ANALYTICS_RAW_PACKET_PARSER_H

#include <raft>

#include "../types/raw_packet.h"
#include "../types/key.h"
#include "../types/packet.h"

namespace starflow {
	namespace kernels {
		class RawPacketParser : public raft::kernel
		{
		public:
			enum class outer_header_type { eth, ip };
			enum class capture_length { full, trunc };

			explicit RawPacketParser(outer_header_type t = outer_header_type::eth,
									 capture_length l = capture_length::full);

			raft::kstatus run() override;

			void set_outer_header_type(outer_header_type t);
			void set_capture_length(capture_length l);

		private:
			outer_header_type _outer_header;
			capture_length _capture_length;
			bool _parse_packet(types::Key& key, types::RawPacket& raw_packet, types::Packet& packet);
		};
	}
}

#endif
