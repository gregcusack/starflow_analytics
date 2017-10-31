
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#include "../types/packet.h"
#include "../types/key.h"
#include "../types/raw_packet.h"

#include "raw_packet_parser.h"

starflow::kernels::RawPacketParser::RawPacketParser(outer_header_type t)
	: _outer_header(t)
{
	input.add_port<types::RawPacket>("in");
	output.add_port<std::pair<types::Key, types::Packet>>("out");
}

raft::kstatus starflow::kernels::RawPacketParser::run()
{
	std::size_t pkt_offset = 0;

	struct ether_header* eth = nullptr;
	struct ip*           ip  = nullptr;
	struct tcphdr*       tcp = nullptr;
	struct udphdr*       udp = nullptr;

	types::Key key;
	types::RawPacket raw_packet {};

	input["in"].pop(raw_packet);

	types::Packet packet(raw_packet.ts, raw_packet.len);

	//TODO: sometimes pl is null after pop from FIFO -- needs debugging, assert for now
	assert(raw_packet.pl != nullptr);

	if (_outer_header == outer_header_type::eth) {
		eth = (ether_header*) raw_packet.pl;
		pkt_offset += sizeof(struct ether_header);

		if (ntohs(eth->ether_type) != ETHERTYPE_IP)
			return raft::proceed;
	}

	ip = (struct ip*) (raw_packet.pl + pkt_offset);

	packet.features.ip_ttl = (unsigned short) ip->ip_ttl;

	pkt_offset += sizeof(struct ip);

	if (ip->ip_p == IPPROTO_UDP) {
		udp = (struct udphdr*) (raw_packet.pl + pkt_offset);
		key = {ip->ip_p, ip->ip_src, ip->ip_dst, udp->uh_sport, udp->uh_dport};
	} else if (ip->ip_p == IPPROTO_TCP) {
		tcp = (struct tcphdr*) (raw_packet.pl + pkt_offset);
		key = {ip->ip_p, ip->ip_src, ip->ip_dst, tcp->th_sport, tcp->th_dport};
		packet.features.tcp_flags = types::Features::tcp_flags_t(tcp->th_flags);
	} else return raft::proceed;

	output["out"].push(std::make_pair(key, packet));

	return raft::proceed;
}
