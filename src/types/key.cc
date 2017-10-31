
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

starflow::types::Key::~Key() { }

std::string starflow::types::Key::_uint32_ip_addr_to_str(uint32_t addr) const
{
	return std::string(inet_ntoa({ .s_addr = addr }));
}
