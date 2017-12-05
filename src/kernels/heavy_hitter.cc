
#include <arpa/inet.h>
#include <algorithm>
#include <functional>
#include <iomanip>

#include "heavy_hitter.h"

#include "../types/key.h"
#include "../types/clfr.h"

starflow::kernels::HeavyHitter::HeavyHitter()
	: _addresses { }
{
	input.add_port<std::pair<types::Key, types::CLFR>>("clfr_in");
}

raft::kstatus starflow::kernels::HeavyHitter::run()
{
	std::pair<starflow::types::Key, starflow::types::CLFR> p {};
	input["clfr_in"].pop(p);

	auto i = _addresses.find(p.first.ip_src);

	if (i == std::end(_addresses)) {
		_addresses.insert(std::make_pair(p.first.ip_src,
			std::make_pair(p.second.n_packets(), p.second.n_bytes())));
	} else {
		i->second.first += p.second.n_packets();
		i->second.second += p.second.n_bytes();
	}

	print_stats();

	return raft::proceed;
}

void starflow::kernels::HeavyHitter::print_stats()
{
	std::cout << "-------------------------------------" << std::endl;
	std::vector<stats> stats_vector;

	for (const entry_t& e : _addresses)
		stats_vector.emplace_back(stats{e.first, e.second.first, e.second.second});

	std::sort(std::begin(stats_vector), std::end(stats_vector), [](const stats& a, const stats& b) {
		return a.pkts > b.pkts;
	});

	for (const stats& s : stats_vector) {
		std::cout << std::setw(15) << _dotted_decimal_ip_addr(s.addr) << " -> " << std::setw(6)
				  << s.pkts << " " << s.bytes << std::endl;
	}
}

std::string starflow::kernels::HeavyHitter::_dotted_decimal_ip_addr(std::uint32_t addr)
{
	return std::string(inet_ntoa({ .s_addr = addr }));
}
