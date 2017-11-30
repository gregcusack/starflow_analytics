
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#include "raw_packet_parser.h"

starflow::modules::RawPacketParser::RawPacketParser(outer_header_type t, capture_length l)
	: _outer_header(t), _capture_length(l) { }

void starflow::modules::RawPacketParser::set_outer_header_type(outer_header_type t)
{
	_outer_header = t;
}

void starflow::modules::RawPacketParser::set_capture_length(capture_length l)
{
	_capture_length = l;
}

std::pair<starflow::types::Key, starflow::types::Packet>
	starflow::modules::RawPacketParser::operator()(const types::RawPacket& raw_packet)
{
	types::Key key;
	types::Packet packet;

	_parse_packet(raw_packet, key, packet);

	return std::make_pair(key, packet);
}


void starflow::modules::RawPacketParser::operator()(const types::RawPacket& raw_packet,
													types::Key& key, types::Packet& packet)
{
	_parse_packet(raw_packet, key, packet);
}

bool starflow::modules::RawPacketParser::_parse_packet(const types::RawPacket& raw_packet,
												  types::Key& key, types::Packet& packet)
{
	std::size_t pkt_offset = 0;

	struct ether_header* eth = nullptr;
	struct ip*           ip  = nullptr;
	struct tcphdr*       tcp = nullptr;
	struct udphdr*       udp = nullptr;

	if (_outer_header == outer_header_type::eth) {
		eth = (ether_header*) raw_packet.pl;
		pkt_offset += sizeof(struct ether_header);

		if (ntohs(eth->ether_type) != ETHERTYPE_IP)
			return false;
	}

	ip = (struct ip*) (raw_packet.pl + pkt_offset);

	packet.features.ip_ttl = ip->ip_ttl;

	pkt_offset += sizeof(struct ip);

	if (ip->ip_p == IPPROTO_UDP) {
		udp = (struct udphdr*) (raw_packet.pl + pkt_offset);
		key = {ip->ip_p, ip->ip_src, ip->ip_dst, ntohs(udp->uh_sport), ntohs(udp->uh_dport)};
	} else if (ip->ip_p == IPPROTO_TCP) {
		tcp = (struct tcphdr*) (raw_packet.pl + pkt_offset);
		key = {ip->ip_p, ip->ip_src, ip->ip_dst, ntohs(tcp->th_sport), ntohs(tcp->th_dport)};
		packet.features.tcp_flags = types::Features::tcp_flags_t(tcp->th_flags);
	} else return false;

	if (_capture_length == capture_length::trunc)
		packet.len = ntohs(ip->ip_len)+(_outer_header == outer_header_type::eth ? sizeof(eth) : 0);
	else
		packet.len = raw_packet.len;

	return true;
}