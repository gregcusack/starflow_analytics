
#include "flow_table.h"

#include <sstream>
#include <iomanip>

starflow::FlowTable::key_t::key_t(u_char ip_p, in_addr ip_src, in_addr ip_dst,
								  unsigned short th_sport, unsigned short th_dport)
	: ip_proto(static_cast<std::uint8_t>(ip_p)),
	  ip_src(static_cast<std::uint32_t>(ip_src.s_addr)),
	  ip_dst(static_cast<std::uint32_t>(ip_dst.s_addr)),
	  th_sport(th_sport),
	  th_dport(th_dport) { }

starflow::FlowTable::key_t::key_t(std::uint8_t ip_proto, std::uint32_t ip_src, std::uint32_t ip_dst,
								  std::uint16_t th_sport, std::uint16_t th_dport)
	: ip_proto(ip_proto),
	  ip_src(ip_src),
	  ip_dst(ip_dst),
	  th_sport(th_sport),
	  th_dport(th_dport) { }

std::string starflow::FlowTable::key_t::str_desc() const
{
	std::stringstream ss;
	ss << std::setw(2) << (int) ip_proto << " " << std::setw(15) << uint32_ip_addr_to_str(ip_src)
	   << " " << std::setw(15) << uint32_ip_addr_to_str(ip_dst) << " " << std::setw(5) << th_sport
	   << " " << std::setw(5) << th_dport;
	return ss.str();
};

bool starflow::FlowTable::key_t::operator<(const starflow::FlowTable::key_t b) const
{
	return std::tie(ip_proto, ip_src, ip_dst, th_sport, th_dport)
		   < std::tie(b.ip_proto, b.ip_src, b.ip_dst, b.th_sport, b.th_dport);
}

starflow::FlowTable::FlowTable(
	std::function<void (key_t, Flow, std::chrono::microseconds, eviction_type)> f,
	std::chrono::seconds timeout_check_period, std::chrono::seconds tcp_timeout,
	std::chrono::seconds udp_timeout)
	: _flows(),
	  _evict_handler(f),
	  _last_timeout_check(0),
	  _timeout_check_period(timeout_check_period),
	  _tcp_timeout(tcp_timeout),
	  _udp_timeout(udp_timeout)
{ }

void starflow::FlowTable::add_packet(std::chrono::microseconds ts, unsigned len,
									 const unsigned char* buf, outer_header_type outer)
{
	key_t flow_key{};
	Flow::packet_t::features_t features{};

	std::size_t pkt_offset = 0;

	struct ether_header* eth = nullptr;
	struct ip*           ip  = nullptr;
	struct tcphdr*       tcp = nullptr;
	struct udphdr*       udp = nullptr;

	if (outer == outer_header_type::eth) {
		eth = (ether_header*) buf;
		pkt_offset += sizeof(struct ether_header);

		if (eth->ether_type != ETHERTYPE_IP)
			return;
	}

	ip = (struct ip*) (buf + pkt_offset);

	features.ttl = (unsigned) ip->ip_ttl;
	pkt_offset += sizeof(struct ip);

	if (ip->ip_p == IPPROTO_UDP) {
		udp = (struct udphdr*) (buf + pkt_offset);
		flow_key = {ip->ip_p, ip->ip_src, ip->ip_dst, udp->uh_sport, udp->uh_dport};
	} else if (ip->ip_p == IPPROTO_TCP) {
		tcp = (struct tcphdr*) (buf + pkt_offset);
		flow_key = {ip->ip_p, ip->ip_src, ip->ip_dst, tcp->th_sport, tcp->th_dport};
	} else return;

	if (_flows.find(flow_key) == std::end(_flows))
		_flows[flow_key] = Flow();

	_flows[flow_key].add_packet(ts, len);

	if (ip->ip_p == IPPROTO_TCP && (tcp->th_flags & TH_FIN) == TH_FIN)
		_evict_flow(flow_key, ts, eviction_type::tcp_fin);

	if (ts.count() - _last_timeout_check.count() >= _timeout_check_period.count())
		_check_flow_timeouts(ts);
}

void starflow::FlowTable::add_packet(std::chrono::microseconds ts, unsigned len, key_t key, bool fin)
{
	if (_flows.find(key) == std::end(_flows))
		_flows[key] = Flow();

	_flows[key].add_packet(ts, len);

	if (fin)
		_evict_flow(key, ts, eviction_type::tcp_fin);

	if (ts.count() - _last_timeout_check.count() >= _timeout_check_period.count())
		_check_flow_timeouts(ts);
}

void starflow::FlowTable::_check_flow_timeouts(std::chrono::microseconds ts)
{
	for (auto& f : _flows) {

		long long int since_last_packet = (ts.count() - f.second.recent_ts().count()) / 1000000;

		if (f.first.ip_proto == IPPROTO_TCP && since_last_packet >= _tcp_timeout.count())
			_evict_flow(f.first, ts, eviction_type::tcp_to);
		else if(f.first.ip_proto == IPPROTO_UDP && since_last_packet >= _udp_timeout.count())
			_evict_flow(f.first, ts, eviction_type::udp_to);
	}

	_last_timeout_check = ts;
}

std::string starflow::FlowTable::uint32_ip_addr_to_str(uint32_t addr)
{
	return std::string(inet_ntoa({ .s_addr = addr }));
}

std::string starflow::FlowTable::eviction_type_to_string(eviction_type e)
{
	switch (e) {
		case eviction_type::udp_to:  return "udp_to";
		case eviction_type::tcp_fin: return "tcp_fin";
		case eviction_type::tcp_to:  return "tcp_to";
	}
}

void starflow::FlowTable::_evict_flow(key_t key, std::chrono::microseconds ts, eviction_type e)
{
	_evict_handler(key, _flows[key], ts, e);
	_flows.erase(key);
}
