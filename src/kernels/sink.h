
#ifndef STARFLOW_KERNELS_SINK
#define STARFLOW_KERNELS_SINK

#include "../types/key.h"
#include "../types/clfr.h"

namespace starflow {
	namespace kernels {
		template<typename T>
		class Sink : public raft::kernel
		{
		public:
			explicit Sink()
				: raft::kernel()
			{
				input.template add_port<T>("in");
			}

			raft::kstatus run() override
			{
				//int count = 0;
				//T t{};
				//input["in"].pop(t);
				std::pair<types::Key, types::CLFR> k_c_pair;
				input["in"].pop(k_c_pair);
				types::Key key = k_c_pair.first;
				types::CLFR clfr = k_c_pair.second;

				unsigned long num_packs = clfr.n_packets();
				unsigned long num_bytes = clfr.n_bytes();
				std::string ip = key.str_desc();	
				std::cout << "Flow: " << ip << ", # packs: " << num_packs << ", # bytes: " << num_bytes << std::endl;

				return (raft::proceed);
			}
		};
	}
}

#endif
