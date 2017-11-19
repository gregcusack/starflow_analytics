
#ifndef STARFLOW_ANALYTICS_HEAVY_HITTER_H
#define STARFLOW_ANALYTICS_HEAVY_HITTER_H

#include <raft>

#include <unordered_map>

namespace starflow {
	namespace kernels {
		class HeavyHitter : public raft::kernel
		{
		public:

			struct stats {
				std::uint32_t addr;
				unsigned long pkts;
				unsigned long bytes;
			};

			HeavyHitter();
			raft::kstatus run() override;

			void print_stats();

		private:

			std::string _dotted_decimal_ip_addr(std::uint32_t addr);

			using entry_t = std::pair<std::uint32_t, std::pair<unsigned long, unsigned long>>;
			std::unordered_map<std::uint32_t, std::pair<unsigned long, unsigned long>> _addresses;
		};
	}
}

#endif
