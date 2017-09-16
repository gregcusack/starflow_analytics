
#include <iostream>
#include <iomanip>
#include <fstream>
#include "import/pcap_replay.h"
#include "flow_table.h"
#include "proto/starflow.pb.h"

int main(int argc, char** argv)
{
	using namespace starflow;

	if (argc != 3) {
		std::cerr << "usage: pcap_flow_exporter <read_from.pcap> <write_to.protobuf>" << std::endl;
		return 1;
	}

	std::string pcap_file_name     = std::string(argv[1]);
	std::string protobuf_file_name = std::string(argv[2]);

	proto::flow_list flow_list;
	std::ofstream of(protobuf_file_name);

	auto start1 = std::chrono::steady_clock::now();
	auto start2 = std::chrono::steady_clock::now();

	FlowTable flow_table([&flow_table, &flow_list, &start2](FlowTable::key_t key, Flow flow,
											   std::chrono::microseconds ts,
											   starflow::FlowTable::eviction_type e) {

		proto::export_flow* flow_proto = flow_list.add_flows();
		flow_proto->set_allocated_flow(new proto::flow(flow.to_proto()));
		flow_proto->set_allocated_key(new proto::key(key.to_proto()));

		int flows_exported = flow_list.flows_size();

		if (flows_exported % 100 == 0) {

			unsigned long flows_in_table = flow_table.count_flows();
			auto now = std::chrono::steady_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start2);

			std::cout << std::setw(7) << flows_exported << " flows, " << std::setw(5) << std::fixed
					  << std::setprecision(2) << (double) flow_list.ByteSize() / 1048576 << " MB, "
					  << duration.count() << " ms, " << flows_in_table << " flows" << std::endl;

			start2 = now;
		}
	});

	starflow::import::PCAPReplay(pcap_file_name, false)(
		[&flow_table](struct pcap_pkthdr* hdr, const unsigned char* buf) {

		flow_table.add_packet(import::PCAPReplay::us_from_timeval(hdr->ts),(unsigned) hdr->caplen,
							  buf, FlowTable::outer_header_type::ip);
	});

	flow_list.SerializeToOstream(&of);
	of.close();

	std::cout << flow_list.flows_size() << " flows written to " << protobuf_file_name << std::endl;

	return 0;
}
