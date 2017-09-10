
## Combined *Flow Analytics Repository

### Components

#### export::PCAPReplay (src/import/pcap_replay.h)

Reads a .pcap file from the file system and replays the packet stream with correct inter-arrival 
times (based on PCAP timestamps). Example usage:

	starflow::import::PCAPReplay pcap_replay("test.pcap");
	pcap_replay([](struct pcap_pkthdr* hdr, const unsigned char* buf) {
		std::cout << (unsigned) hdr->caplen << " bytes" << std::endl;
	}); // blocks

#### FlowTable (src/flow_table.h)

Implements a configurable flow table based on the L3/L4 5-tuple. Usage example:

	starflow::FlowTable flow_table([](starflow::FlowTable::key_t key, starflow::Flow flow,
									  std::chrono::microseconds ts,
									  starflow::FlowTable::eviction_type e) {

		std::cout <<  ts.count() << " " << key.str_desc() << " " << std::setw(7)
				  << starflow::FlowTable::eviction_type_to_string(e)
				  << " " << flow.str_desc() << std::endl;
	});
	
	flow_table.add_packet(...);
	
	
#### RedisFlowPublisher (src/redis_flow_publisher.h)

Implements a Redis client that publishes flow records to a topic.