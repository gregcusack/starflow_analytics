
#include <raft>

#include <raftio>

#include "kernels/ip_source_timestamps.h"
#include "kernels/printer.h"
#include "kernels/redis_flow_subscriber.h"
#include "kernels/counter.h"
#include "kernels/sink.h"

std::ostream& operator<<(std::ostream& os, const starflow::kernels::IPSourceTimestamps::output_t& t)
{
	return os;
}

int main(int argc, char** argv)
{
	starflow::kernels::RedisFlowSubscriber subscriber("127.0.0.1", 6379, "starflow");
	starflow::kernels::IPSourceTimestamps ip_source_timestamps;
	starflow::kernels::Sink<starflow::kernels::IPSourceTimestamps::output_t> sink;

	raft::map m;
	m += subscriber >> ip_source_timestamps >> sink;
	m.exe();

	return 0;
}
