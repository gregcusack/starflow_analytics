
#ifndef STARFLOW_ANALYTICS_KERNELS_LIVE_CAPTURE_H
#define STARFLOW_ANALYTICS_KERNELS_LIVE_CAPTURE_H

#include <raft>
#include "../modules/live_capture.h"

namespace starflow {
	namespace kernels {
		class LiveCapture : public raft::kernel
		{
		public:
			explicit LiveCapture(const std::string& dev);
			raft::kstatus run() override;
		private:
			modules::LiveCapture _live_capture;
		};
	}
}

#endif
