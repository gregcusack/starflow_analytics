
#include "packet.h"

starflow::types::Packet::Packet(std::chrono::microseconds ts, unsigned int len)
	: ts(ts), len(len)
{ }

starflow::types::Packet::Packet(unsigned long ts, unsigned int len)
	: ts(ts), len(len)
{ }

std::string starflow::types::Packet::str_desc() const
{
	std::string desc = "starflow::types::Packet(ts=" + std::to_string(ts.count()) + ", len=" + std::to_string(len) + ")";
	return desc;
}

starflow::types::Packet::~Packet()
{ }
