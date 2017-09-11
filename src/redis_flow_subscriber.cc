
#include "redis_flow_subscriber.h"

starflow::RedisFlowSubscriber::RedisFlowSubscriber(const std::string& redis_host,
	unsigned redis_port, const std::string& topic, starflow::RedisFlowSubscriber::cb_t&& cb)
	throw (std::runtime_error)
	: starflow::RedisSubscriber(redis_host, redis_port, topic),
	  _callback(cb) { }

void starflow::RedisFlowSubscriber::operator()()
{
	_redis.subscribe(_topic, [this](const std::string& chan, const std::string& msg) {
		proto::export_flow export_flow;
		export_flow.ParseFromString(msg);
		Flow flow = Flow::from_proto(export_flow.flow());
		FlowTable::key_t key = FlowTable::key_t::from_proto(export_flow.key());
		_callback(key, flow);
	});
	_redis.commit();

	while (!_exit) { }
}
