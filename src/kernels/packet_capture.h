
#ifndef STARFLOW_ANALYTICS_PACKET_CAPTURE_H
#define STARFLOW_ANALYTICS_PACKET_CAPTURE_H

#include <raft>
#include <pcap.h>

namespace starflow {
	namespace kernels {
		class PacketCapture : public raft::kernel
		{
		public:
			explicit PacketCapture(const std::string& dev);
			raft::kstatus run() override;

		private:
			void _connect(const std::string& dev) throw(std::runtime_error);
			pcap_t* _pcap;
		};
	}
}

#endif
