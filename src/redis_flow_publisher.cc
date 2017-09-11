
#include "redis_flow_publisher.h"

starflow::RedisFlowPublisher::RedisFlowPublisher(std::string redis_host, unsigned redis_port)
	throw (std::runtime_error)
	: RedisClient(redis_host, redis_port) { }

void starflow::RedisFlowPublisher::publish(FlowTable::key_t key, Flow flow)
	throw (std::runtime_error)
{
	_redis.publish("test", "hello");
	_redis.sync_commit();
}
