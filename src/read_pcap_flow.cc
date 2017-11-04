#include <fstream>
#include <iomanip>
#include "import/pcap_replay.h"
#include "flow_table.h"
//#include <google/protobuf/util/time_util.h>
#include <iostream>
#include <string>

#include "proto/starflow.pb.h"

void ListFlows(const starflow::proto::flow_list& flow_list) {
	for (int i=0; i < flow_list.flows(); i++ ) {

	}
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "Usage:  " << argv[0] << " flow_file.protobuf" << std::endl;
		return -1;
	}
	starflow::proto::flow_list flow_list;

	std::fstream input(argv[1], std::ios::in | std::ios::binary);
	if (!flow_list.ParseFromIstream(&input)) {
		std::cerr << "Failed to parse Flow Table" << std::endl;
		return -1;
	}
	ListFlows(flow_list);

}

//use flow file replay
//look at PCAP replay function along with redis publisher example
