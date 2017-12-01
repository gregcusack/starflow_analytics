
#include "key.h"
#include "../helpers.h"
#include <sstream>
#include <string>
#include <iomanip>
#include <arpa/inet.h>

starflow::types::Key::Key(u_char ip_p, in_addr ip_src, in_addr ip_dst, unsigned short th_sport,
	unsigned short th_dport)
	: ip_proto(static_cast<std::uint8_t>(ip_p)),
	  ip_src(static_cast<std::uint32_t>(ip_src.s_addr)),
	  ip_dst(static_cast<std::uint32_t>(ip_dst.s_addr)),
	  th_sport(th_sport),
	  th_dport(th_dport)
{ }

starflow::types::Key::Key(std::uint8_t ip_p, std::uint32_t ip_src, std::uint32_t ip_dst,
	std::uint16_t th_sport, std::uint16_t th_dport)
	: ip_proto(ip_p),
	  ip_src(ip_src),
	  ip_dst(ip_dst),
	  th_sport(th_sport),
	  th_dport(th_dport)
{ }

std::string starflow::types::Key::str_desc() const
{
	std::string desc = "starflow::types::Key(ip_proto=" + std::to_string(ip_proto) + ", ip_src="
		+ helpers::uint32_ip_addr_to_str(ip_src) + ", ip_dst="
		+ helpers::uint32_ip_addr_to_str(ip_dst) + ", th_sport="
		+ std::to_string(th_sport) + ", th_dport=" + std::to_string(th_dport) + ")";

	return desc;
};

bool starflow::types::Key::operator==(const starflow::types::Key& b) const
{
	return std::tie(ip_proto, ip_src, ip_dst, th_sport, th_dport)
		   == std::tie(b.ip_proto, b.ip_src, b.ip_dst, b.th_sport, b.th_dport);
}

bool starflow::types::Key::operator<(const starflow::types::Key& b) const
{
	return std::tie(ip_proto, ip_src, ip_dst, th_sport, th_dport)
		<  std::tie(b.ip_proto, b.ip_src, b.ip_dst, b.th_sport, b.th_dport);
}
