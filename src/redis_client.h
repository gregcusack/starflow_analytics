
#ifndef STARFLOW_ANALYTICS_REDIS_CLIENT_H
#define STARFLOW_ANALYTICS_REDIS_CLIENT_H

#include <stdexcept>
#include <string>

#include <cpp_redis/cpp_redis>

namespace starflow {
	class RedisClient
	{
	public:
		RedisClient(std::string redis_host, unsigned redis_port)
			throw (std::runtime_error);

		RedisClient(const RedisClient&)             = delete;
		RedisClient& operator=(const RedisClient&)  = delete;
		RedisClient(RedisClient&&)                  = delete;
		RedisClient& operator=(RedisClient&&)       = delete;

		void disconnect();
		virtual ~RedisClient();

	protected:
		cpp_redis::redis_client _redis;
	};
}


#endif
