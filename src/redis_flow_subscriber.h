
#ifndef STARFLOW_ANALYTICS_REDIS_FLOW_SUBSCRIBER_H
#define STARFLOW_ANALYTICS_REDIS_FLOW_SUBSCRIBER_H

#include <string>
#include <stdexcept>
#include <functional>

#include <cpp_redis/cpp_redis>

#include "flow.h"
#include "flow_table.h"
#include "redis_subscriber.h"

namespace starflow {
	class RedisFlowSubscriber : public RedisSubscriber
	{
		using cb_t = std::function<void (starflow::FlowTable::key_t, starflow::Flow)>;

	public:
		RedisFlowSubscriber(std::string redis_host, unsigned redis_port, std::string topic, cb_t cb)
			throw (std::runtime_error);

		void operator()();

	private:
		cb_t _callback;
	};
}

#endif
