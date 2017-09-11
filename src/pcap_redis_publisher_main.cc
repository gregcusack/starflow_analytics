
#include "import/pcap_replay.h"
#include "flow_table.h"
#include "redis_flow_publisher.h"

int main(int argc, char** argv)
{
	using namespace starflow;

	if (argc < 2 || argc > 5) {
		std::cerr << "usage: pcap_redis_publisher <read_from.pcap> [<topic>] [<redis_host>]"
				  << " [<redis_port>]" << std::endl;
		return 1;
	}

	std::string pcap_file_name = std::string(argv[1]);
	std::string redis_topic    = (argc >= 3) ? std::string(argv[2]) : "starflow";
	std::string redis_host     = (argc >= 4) ? std::string(argv[3]) : "127.0.0.1";
	unsigned short redis_port  = (argc == 5) ? (unsigned short) atoi(argv[4]) : 6379;

	RedisFlowPublisher publisher(redis_host, redis_port);

	FlowTable flow_table([&redis_topic, &publisher](FlowTable::key_t key, Flow flow,
		std::chrono::microseconds ts, starflow::FlowTable::eviction_type e) {
		publisher.publish(redis_topic, key, flow);
	});

	starflow::import::PCAPReplay(pcap_file_name, false)(
		[&flow_table](struct pcap_pkthdr* hdr, const unsigned char* buf) {

		flow_table.add_packet(import::PCAPReplay::us_from_timeval(hdr->ts), (unsigned) hdr->caplen,
							  buf, FlowTable::outer_header_type::ip);
	});
}
