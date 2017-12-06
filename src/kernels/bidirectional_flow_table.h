#ifndef STARFLOW_ANALYTICS_KERNELS_BIDIRECTIONAL_FLOW_TABLE_H
#define STARFLOW_ANALYTICS_KERNELS_BIDIRECTIONAL_FLOW_TABLE_H

#include <raft>

#include "../modules/bidirectional_flow_table.h"

namespace starflow{
	namespace kernels {
		class BiFlowTable : public raft::kernel {
		public:
			BiFlowTable();
			raft::kstatus run() override;

		private:
			modules::BiFlowTable _bi_flow_table;
		};
	}
}

#endif