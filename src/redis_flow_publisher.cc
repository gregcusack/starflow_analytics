
#include "redis_flow_publisher.h"
#include "proto/starflow.pb.h"


starflow::RedisFlowPublisher::RedisFlowPublisher(const std::string& redis_host, unsigned redis_port)
	throw (std::runtime_error)
	: RedisClient(redis_host, redis_port) { }

void starflow::RedisFlowPublisher::publish(const std::string& topic, FlowTable::key_t key, Flow flow)
	throw (std::runtime_error)
{
	proto::export_flow ex;
	ex.set_allocated_key(new proto::key(key.to_proto()));
	ex.set_allocated_flow(new proto::flow(flow.to_proto()));
	_redis.publish(topic, ex.SerializeAsString());
	_redis.sync_commit();
}
