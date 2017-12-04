
#ifndef STARFLOW_ANALYTICS_KERNELS_TZSP_RECEIVER_H
#define STARFLOW_ANALYTICS_KERNELS_TZSP_RECEIVER_H

#include <raft>
#include <stdexcept>
#include "../modules/tzsp_receiver.h"

namespace starflow {
	namespace kernels {
		class TZSPReceiver : public  raft::kernel
		{
		public:
			explicit TZSPReceiver(unsigned short listen_port);
			raft::kstatus run() override;
		private:
			modules::TZSPReceiver _tzsp_receiver;
		};
	}
}

#endif
