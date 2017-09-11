
#include "redis_client.h"

starflow::RedisClient::RedisClient(std::string redis_host, unsigned redis_port)
throw (std::runtime_error)
{
	try {
		_redis.connect(redis_host, redis_port);
	} catch (cpp_redis::redis_error& e) {
		throw std::runtime_error("RedisFlowPublisher: " + std::string(e.what()));
	}

	_redis.set_callback_runner(
		[this](cpp_redis::reply& r, const cpp_redis::redis_client::reply_callback_t& c) {
			std::cout << "callback" << std::endl;
		});
}

void starflow::RedisClient::disconnect()
{
	_redis.disconnect();
}

starflow::RedisClient::~RedisClient()
{
	if (_redis.is_connected())
		_redis.disconnect();

}