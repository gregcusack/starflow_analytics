
## Combined *Flow Analytics Repository

### Dependencies

* libraft [[1]](https://github.com/RaftLib/RaftLib)
* libpcap [[2]](http://www.tcpdump.org)
* libcpp_redis [[3]](https://github.com/Cylix/cpp_redis)
* libprotobuf & protoc [[4]](https://github.com/google/protobuf)

---
### Build

Requires C++14 compatible compiler.

    mkdir build && cd build
    cmake ..
    make

---
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

#### RedisFlowSubscriber (src/redis_flow_subscriber.h)

Implements a Redis client that connects to a redis server and reads flow records from a pub/sub channel.

#### FlowFileReplay (src/import/flow_file_replay.h)

Reads a protobuf file and replays the flows based on eviction timestamps.

---
### Docker Redis Image
        
    docker build .
    docker run -itp 6379:6379 redis

---
### References

[1] https://github.com/RaftLib/RaftLib

[2] http://www.tcpdump.org

[3] https://github.com/Cylix/cpp_redis

[4] https://github.com/google/protobuf
