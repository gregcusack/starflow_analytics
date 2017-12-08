#ifndef STARFLOW_ANALYTICS_KERNELS_BIDIRECTIONAL_FLOW_TABLE_H
#define STARFLOW_ANALYTICS_KERNELS_BIDIRECTIONAL_FLOW_TABLE_H

#include "../types/key.h"
#include "../types/clfr.h"

#include <raft>
#include <map>
#include <stdexcept>
#include <iostream>


//#include "../modules/bidirectional_flow_table.h"

namespace starflow{
	namespace kernels {
		class BiFlowTable : public raft::kernel {
			using bi_flow_table_t           = std::map<types::Key, std::pair<types::CLFR, types::CLFR>>;
			using exported_bi_flows_table_t = std::list<std::pair<types::Key, types::CLFR>>;
			using export_bi_flow_callback_t = std::function<void (types::CLFR)>;
		public:
			BiFlowTable();
			raft::kstatus run() override;
			bi_flow_table_t::iterator add_clfr(types::CLFR clfr)
				throw(std::logic_error);

		private:
			bi_flow_table_t _active_flows					= {};
			//modules::BiFlowTable _bi_flow_table;

		};
	}
}

#endif