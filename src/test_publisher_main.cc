
#include "redis_flow_publisher.h"

int main(int argc, char** argv)
{
	starflow::RedisFlowPublisher publisher("127.0.0.1", 6379);
	publisher.publish("test", starflow::FlowTable::key_t(), starflow::Flow());
	return 0;
}
