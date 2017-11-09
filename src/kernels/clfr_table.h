
#ifndef STARFLOW_ANALYTICS_FLOW_TABLE_H
#define STARFLOW_ANALYTICS_FLOW_TABLE_H

#include <raft>
#include <pcap.h>
#include "../types/key.h"
#include "../types/clfr.h"

namespace starflow {
	namespace kernels {
		class CLFRTable : public raft::kernel
		{
		public:
			explicit CLFRTable();
			raft::kstatus run() override;

		private:
			std::map<types::Key, types::CLFR> _flows;

			void _check_timeouts();
			void _evict_flow(const types::Key& k);
		};
	}
}

#endif
