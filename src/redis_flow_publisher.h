
#ifndef STARFLOW_REDIS_FLOW_PUBLISHER_H
#define STARFLOW_REDIS_FLOW_PUBLISHER_H

#include "flow.h"
#include "flow_table.h"
#include "redis_client.h"

namespace starflow {

	class RedisFlowPublisher : public RedisClient
	{
	public:
		RedisFlowPublisher(std::string redis_host, unsigned redis_port)
			throw (std::runtime_error);

		void publish(FlowTable::key_t key, Flow flow) throw (std::runtime_error);
	};
}

#endif
