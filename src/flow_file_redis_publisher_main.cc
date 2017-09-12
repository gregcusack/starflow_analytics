
#include "import/flow_file_replay.h"
#include "redis_flow_publisher.h"

int main(int argc, char** argv)
{
	using namespace starflow;

	if (argc < 2 || argc > 5) {
		std::cerr << "usage: flow_file_redis_publisher <read_from.protobuf> [<topic>]"
				  << " [<redis_host>] [<redis_port>]" << std::endl;
		return 1;
	}

	std::string protobuf_file_name = std::string(argv[1]);
	std::string redis_topic        = (argc >= 3) ? std::string(argv[2]) : "starflow";
	std::string redis_host         = (argc >= 4) ? std::string(argv[3]) : "127.0.0.1";
	unsigned short redis_port      = (argc == 5) ? (unsigned short) atoi(argv[4]) : 6379;

	RedisFlowPublisher publisher(redis_host, redis_port);

	starflow::import::FlowFileReplay flow_file_replay(protobuf_file_name);
	flow_file_replay([&redis_topic, &publisher](FlowTable::key_t key, Flow flow) {
		publisher.publish(redis_topic, key, flow);
	});

	return 0;
}
