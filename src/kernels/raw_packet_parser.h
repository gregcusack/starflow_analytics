
#ifndef STARFLOW_ANALYTICS_RAW_PACKET_PARSER_H
#define STARFLOW_ANALYTICS_RAW_PACKET_PARSER_H

#include <raft>

namespace starflow {
	namespace kernels {
		class RawPacketParser : public raft::kernel
		{
		public:
			enum class outer_header_type {
				eth, ip
			};

			explicit RawPacketParser(outer_header_type t = outer_header_type::eth);
			raft::kstatus run() override;

		private:
			outer_header_type _outer_header;
		};
	}
}

#endif
