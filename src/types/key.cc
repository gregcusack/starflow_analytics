
#include "key.h"

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
		+ _uint32_ip_addr_to_str(ip_src) + ", ip_dst=" + _uint32_ip_addr_to_str(ip_dst)
		+ ", th_sport=" + std::to_string(th_sport) + ", th_dport=" + std::to_string(th_dport) + ")";

	return desc;
};

//function to print key to CSV for feature extraction
std::string starflow::types::Key::str_desc_for_df() const {
	std::string desc = std::to_string(ip_proto) + "," + _uint32_ip_addr_to_str(ip_src) + "," + _uint32_ip_addr_to_str(ip_dst) + "," + std::to_string(th_sport) + "," + std::to_string(th_dport);
	return desc;
}

bool starflow::types::Key::operator<(starflow::types::Key b) const
{
	return std::tie(ip_proto, ip_src, ip_dst, th_sport, th_dport)
		<  std::tie(b.ip_proto, b.ip_src, b.ip_dst, b.th_sport, b.th_dport);
}

std::string starflow::types::Key::_uint32_ip_addr_to_str(uint32_t addr) const
{
	return std::string(inet_ntoa({ .s_addr = addr }));
}

std::string starflow::types::Key::get_ip_src() const {
	return _uint32_ip_addr_to_str(ip_src);
}
std::string starflow::types::Key::get_dst_src() const {
	return _uint32_ip_addr_to_str(ip_dst);
}
