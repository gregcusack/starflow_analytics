
#ifndef STARFLOW_ANALYTICS_PCAP_FILE_READER_H
#define STARFLOW_ANALYTICS_PCAP_FILE_READER_H

#include <raft>
#include <pcap.h>

#include "../modules/pcap_reader.h"
#include "../types/raw_packet.h"

namespace starflow {
	namespace kernels {
		class PCAPFileReader : public raft::kernel
		{
		public:
			explicit PCAPFileReader(const std::string& file_name);
			raft::kstatus run() override;

		private:
			void _read_packet(starflow::types::RawPacket p);
			modules::PCAPReader _pcap_reader;
		};
	}
}

#endif
