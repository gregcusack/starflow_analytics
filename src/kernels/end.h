
#ifndef STARFLOW_KERNELS_END
#define STARFLOW_KERNELS_END

#include "../types/key.h"
#include "../types/clfr.h"
#include <string>
#include <iostream>
#include <fstream>
#include <raft>

namespace starflow {
	namespace kernels {
		class End : public raft::kernel {
		public:
			explicit End();
			raft::kstatus run() override;
		};
	}
}

#endif
