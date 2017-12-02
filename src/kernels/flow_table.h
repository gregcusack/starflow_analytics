
#ifndef STARFLOW_ANALYTICS_KERNELS_FLOW_TABLE_H
#define STARFLOW_ANALYTICS_KERNELS_FLOW_TABLE_H

#include <raft>

#include "../modules/flow_table.h"

namespace starflow {
	namespace kernels {
		class FlowTable : public raft::kernel
		{
		public:
			FlowTable();
			raft::kstatus run() override;

		private:
			void _flow_export(types::Key k, types::CLFR f);
			modules::FlowTable _flow_table;
		};
	}
}

#endif
