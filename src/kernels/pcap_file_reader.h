
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
			explicit PCAPFileReader(const std::string& file_name)
				: _pcap_reader()
			{
				_pcap_reader.set_file_name(file_name);
				_pcap_reader.set_callback([this](types::RawPacket p) {
					this->read_packet(std::move(p));
				});
				output.add_port<starflow::types::RawPacket>("out");
			}

			raft::kstatus run() override
			{
				_pcap_reader();
				return(raft::proceed);
			}

			void read_packet(starflow::types::RawPacket p)
			{
				output["out"].push(p);
			}

		private:
			modules::PCAPReader _pcap_reader;
		};
	}
}

#endif
