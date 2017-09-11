
#ifndef STARFLOW_ANALYTICS_REDIS_SUBSCRIBER_H
#define STARFLOW_ANALYTICS_REDIS_SUBSCRIBER_H

#include <stdexcept>
#include <string>

#include <cpp_redis/cpp_redis>

namespace starflow {
	class RedisSubscriber
	{
	public:
		RedisSubscriber(std::string redis_host, unsigned redis_port, std::string topic)
			throw (std::runtime_error);

		RedisSubscriber(const RedisSubscriber&)             = delete;
		RedisSubscriber& operator=(const RedisSubscriber&)  = delete;
		RedisSubscriber(RedisSubscriber&&)                  = delete;
		RedisSubscriber& operator=(RedisSubscriber&&)       = delete;

		virtual void operator()() = 0;

		void disconnect();
		virtual ~RedisSubscriber();

	protected:
		cpp_redis::redis_subscriber _redis;
		std::string _topic;
		bool _exit = false;
	};
}



#endif
