
#ifndef STARFLOW_ANALYTICS_FLOW_FILE_REPLAY_H
#define STARFLOW_ANALYTICS_FLOW_FILE_REPLAY_H

#include <string>
#include <functional>
#include <chrono>

#include "../flow_table.h"
#include "../flow.h"

namespace starflow {
	namespace import {
		class FlowFileReplay
		{
		public:
			using callback_t = std::function<void (FlowTable::key_t, Flow)>;

			FlowFileReplay()                                                 = delete;
			FlowFileReplay(const std::string& file_name, bool sleep = true);
			FlowFileReplay(const FlowFileReplay&)                            = default;
			FlowFileReplay& operator=(const FlowFileReplay&)                 = default;
			FlowFileReplay(FlowFileReplay&&)                                 = default;
			FlowFileReplay& operator=(FlowFileReplay&&)                      = default;

			void operator()(callback_t f);

		private:
			unsigned long _i                   = 0;
			proto::flow_list _flow_list        = {};
			std::chrono::microseconds _last_ts = std::chrono::microseconds(0);
			bool _sleep                        = true;
		};
	}
}


#endif