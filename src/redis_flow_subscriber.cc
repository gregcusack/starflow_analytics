
#include "redis_flow_subscriber.h"

starflow::RedisFlowSubscriber::RedisFlowSubscriber(std::string redis_host, unsigned redis_port,
										 std::string topic, starflow::RedisFlowSubscriber::cb_t cb)
	throw (std::runtime_error)
	: starflow::RedisSubscriber(redis_host, redis_port, topic),
	  _callback(cb) { }

void starflow::RedisFlowSubscriber::operator()()
{
	_redis.subscribe(_topic, [](const std::string& chan, const std::string& msg) {
		std::cout << chan << " " << msg << std::endl;
	});
	_redis.commit();

	while (!_exit) { }
}
