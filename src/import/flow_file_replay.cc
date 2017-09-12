
#include "flow_file_replay.h"

#include <thread>
#include <fstream>

starflow::import::FlowFileReplay::FlowFileReplay(const std::string& file_name, bool sleep)
	: _i(0), _sleep(sleep)
{
	std::ifstream ifs;

	ifs.open(file_name);
	_flow_list.ParseFromIstream(&ifs);
	ifs.close();
}

void starflow::import::FlowFileReplay::operator()(callback_t f)
{
	for (auto& export_flow : _flow_list.flows()) {
		Flow flow = Flow::from_proto(export_flow.flow());
		FlowTable::key_t key = FlowTable::key_t::from_proto(export_flow.key());

		if (_i > 0 && _sleep) {
			using us = std::chrono::microseconds;
			auto d = std::chrono::duration_cast<us>(flow.eviction_ts() - _last_ts);
			std::this_thread::sleep_for(d);
		}

		_last_ts = flow.eviction_ts();
		f(key, flow);
		_i++;
	}
}
