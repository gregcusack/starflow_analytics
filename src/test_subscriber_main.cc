
#include "redis_flow_subscriber.h"

int main(int argc, char** argv)
{
	starflow::RedisFlowSubscriber("127.0.0.1", 6379, "test",
		[](starflow::FlowTable::key_t k, starflow::Flow f) {

		std::cout << k.str_desc() << " / " << f.str_desc() << std::endl;
	})();

	return 0;
}
