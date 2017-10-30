
#ifndef STARFLOW_ANALYTICS_PCAP_FILE_READER_H
#define STARFLOW_ANALYTICS_PCAP_FILE_READER_H

#include <raft>
#include <pcap.h>

namespace starflow {
	namespace kernels {
		class PCAPFileReader : public raft::kernel
		{
		public:
			explicit PCAPFileReader(const std::string& file_name);
			raft::kstatus run() override;

		private:
			void _open(const std::string& dev) throw(std::runtime_error);
			pcap_t* _pcap = nullptr;
		};
	}
}

#endif
