
#include "import/pcap_replay.h"
#include "flow_table.h"
#include "redis_flow_publisher.h"

int main(int argc, char** argv)
{
	using namespace starflow;

	//TODO: read from argv
	std::string pcap_file_name = "/Users/olli/Downloads/caida2015_02_dirA.pcap";
	std::string redis_topic    = "starflow";
	std::string redis_host     = "127.0.0.1";
	unsigned short redis_port  = 6379;

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
