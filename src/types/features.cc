
#include "features.h"

starflow::types::Features::tcp_flags_t::tcp_flags_t()
	: _flags(0) { }

starflow::types::Features::tcp_flags_t::tcp_flags_t(unsigned char flags)
	: _flags(flags) { }

bool starflow::types::Features::tcp_flags_t::is_urg() const
{
	return (_flags & _urg) == _urg;
}

bool starflow::types::Features::tcp_flags_t::is_ack() const
{
	return (_flags & _ack) == _ack;
}

bool starflow::types::Features::tcp_flags_t::is_psh() const
{
	return (_flags & _psh) == _psh;
}

bool starflow::types::Features::tcp_flags_t::is_rst() const
{
	return (_flags & _rst) == _rst;
}

bool starflow::types::Features::tcp_flags_t::is_syn() const
{
	return (_flags & _syn) == _syn;
}

bool starflow::types::Features::tcp_flags_t::is_fin() const
{
	return (_flags & _fin) == _fin;
}

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
