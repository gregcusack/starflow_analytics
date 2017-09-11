
#include "redis_subscriber.h"

starflow::RedisSubscriber::RedisSubscriber(std::string redis_host, unsigned redis_port)
	throw (std::runtime_error)
{
	try {
		_redis.connect(redis_host, redis_port);
	} catch (cpp_redis::redis_error& e) {
		throw std::runtime_error("RedisSubscriber: " + std::string(e.what()));
	}
}

void starflow::RedisSubscriber::disconnect()
{
	_redis.disconnect();
}

starflow::RedisSubscriber::~RedisSubscriber()
{
	if (_redis.is_connected())
		_redis.disconnect();
}
