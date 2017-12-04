
#include "format_helpers.h"

#include <arpa/inet.h>

std::string starflow::etc::format_helpers::uint32_ip_addr_to_str(const uint32_t& addr)
{
	return std::string(inet_ntoa({ .s_addr = addr }));
}
