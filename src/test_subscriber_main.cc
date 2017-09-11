
#include "redis_flow_subscriber.h"

int main(int argc, char** argv)
{
	starflow::RedisFlowSubscriber("127.0.0.1", 6379, "test",
		[](starflow::FlowTable::key_t, starflow::Flow) {
		std::cout << "Flow" << std::endl;
	})();

	return 0;
}
