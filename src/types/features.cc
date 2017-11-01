
#include "features.h"

starflow::types::Features::tcp_flags_t::tcp_flags_t()
	: _flags(0) { }

starflow::types::Features::tcp_flags_t::tcp_flags_t(unsigned char flags)
	: _flags(flags) { }

std::string starflow::types::Features::tcp_flags_t::str_desc() const
{
	std::string desc = "starflow::types::Features::tcp_flags_t(";
	desc += is_urg() ? " urg" : "";
	desc += is_ack() ? " ack" : "";
	desc += is_psh() ? " psh" : "";
	desc += is_rst() ? " rst" : "";
	desc += is_syn() ? " syn" : "";
	desc += is_fin() ? " fin" : "";
	return desc + " )";
}

starflow::types::Features::~Features()
{ }
