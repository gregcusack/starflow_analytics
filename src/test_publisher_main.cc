
#include "redis_flow_publisher.h"

int main(int argc, char** argv)
{
	starflow::RedisFlowPublisher publisher("localhost", 6379);
	publisher.publish(starflow::FlowTable::key_t(), starflow::Flow());
	return 0;
}
