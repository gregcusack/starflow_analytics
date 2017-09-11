
#include <raft>

#include "kernels/printer.h"
#include "kernels/redis_flow_subscriber.h"
#include "kernels/counter.h"

int main(int argc, char** argv)
{
	starflow::kernels::RedisFlowSubscriber subscriber("127.0.0.1", 6379, "starflow");
	starflow::kernels::Counter<std::pair<starflow::FlowTable::key_t, starflow::Flow>> counter;
	starflow::kernels::Printer<unsigned long long> printer;

	raft::map m;
	m += subscriber >> counter >> printer;
	m.exe();

	return 0;
}
