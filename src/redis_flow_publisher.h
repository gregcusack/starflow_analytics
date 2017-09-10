
#ifndef STARFLOW_REDIS_FLOW_PUBLISHER_H
#define STARFLOW_REDIS_FLOW_PUBLISHER_H

#include <stdexcept>
#include <string>

#include "flow.h"
#include "flow_table.h"

namespace starflow {

	class RedisFlowPublisher
	{
		RedisFlowPublisher(std::string redis_host, int redis_port, std::string topic)
			throw (std::runtime_error);

		RedisFlowPublisher(const RedisFlowPublisher&)            = delete;
		RedisFlowPublisher& operator=(const RedisFlowPublisher&) = delete;
		RedisFlowPublisher(RedisFlowPublisher&&)                 = default;
		RedisFlowPublisher& operator=(const RedisFlowPublisher&) = default;

		void publish(FlowTable::key_t key, Flow flow) const
			throw (std::runtime_error);

		~RedisFlowPublisher() = default;
	};
}

#endif
