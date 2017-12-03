
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

int count = 0;
int count_1 = 0;
int count_2 = 0;
int count_3 = 0;
int count_4 = 0;
int count_5 = 0;
int total = 0;

starflow::kernels::RawPacketParser::RawPacketParser(outer_header_type t)
	: _outer_header(t)
{
	input.add_port<types::RawPacket>("in");
	output.add_port<std::pair<types::Key, types::Packet>>("out");
}

raft::kstatus starflow::kernels::RawPacketParser::run()
{
	types::Key key;
	types::RawPacket raw_packet {};

	input["in"].pop(raw_packet);
	types::Packet packet(raw_packet.ts, raw_packet.len);

	if (_parse_packet(key, raw_packet, packet)) {
		//std::cout << key.get_ip_src() << std::endl;
		
		/*if(key.get_ip_src() == "209.124.66.6") {
			std::cout << "src IP: " << key.get_ip_src() << std::endl;
			std::cout << "packet length: " << raw_packet.len << std::endl;
		}*/
		output["out"].push(std::make_pair(key, packet));
	}
	return raft::proceed;
}

bool starflow::kernels::RawPacketParser
	::_parse_packet(types::Key& key, types::RawPacket& raw_packet, types::Packet& packet)
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
		//std::cout << key.get_ip_src() << std::endl;
		if(key.get_ip_src() == "209.124.66.6") {
			total++;
			count++;
			std::cout << "count: " << count << std::endl;
		}
		if(key.get_ip_src() == "10.11.9.101") {
			total++;
			count_1++;
			std::cout << "count_1: " << count_1 << std::endl;
		}
		if(key.get_ip_src() == "203.146.43.65") {
			total++;
			count_2++;
			std::cout << "count_2: " << count_2 << std::endl;
		}
		if(key.get_ip_src() == "192.185.57.176") {
			total++;
			count_3++;
			std::cout << "count_3: " << count_3 << std::endl;
		}
		if(key.get_ip_src() == "49.51.33.178") {
			total++;
			count_4++;
			std::cout << "count_4: " << count_4 << std::endl;
		}
		if(key.get_ip_src() == "10.10.11.101") {
			total++;
			count_5++;
			std::cout << "count_4: " << count_4 << std::endl;
		}
		//total++;
		std::cout << "Total: " << total << std::endl;
		//if(key.get_ip_src() == "209.124.66.6") {
		/*if(key.get_ip_src() == "203.146.43.65") {
			std::cout << "src IP: " << key.get_ip_src() << std::endl;
			std::cout << "packet length: " << raw_packet.len << std::endl;
			std::cout << "packet ts: " << packet.ts.count() << std::endl;
		}*/
	} else {
		return false;
	}
	

	return true;
}
