
#ifndef STARFLOW_ANALYTICS_TZSP_RECEIVER_H
#define STARFLOW_ANALYTICS_TZSP_RECEIVER_H

#include <raft>
#include <stdexcept>

namespace starflow {
	namespace kernels {
		class TZSPReceiver : public  raft::kernel
		{
			struct tzsp_header {
				uint8_t  version;
				uint8_t  type;
				uint16_t encap;
			};

			struct tzsp_tag {
				uint8_t type;
				uint8_t length;
				char  data[];
			};

		public:
			explicit TZSPReceiver(unsigned short listen_port) throw(std::runtime_error);
			raft::kstatus run() override;
			~TZSPReceiver() override;

		private:
			unsigned short _listen_port;
			int            _sock = -1;
		};
	}
}

#endif
