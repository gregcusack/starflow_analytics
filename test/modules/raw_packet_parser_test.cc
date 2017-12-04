
#include <catch.h>
#include <iostream>
#include "../../src/modules/raw_packet_parser.h"
#include "../../src/etc/format_helpers.h"

#include "../include/raw_packet_data.h"

using namespace starflow;

TEST_CASE("RawPacketParser", "[modules::RawPacketParser]")
{
	unsigned char pkt_data1[] = {
		// 62B, EthII, IP4, TCP [SYN], 145.254.160.237 -> 65.208.228.223, 3372 -> 80
		0xfe, 0xff, 0x20, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x45,
		0x00, 0x00, 0x30, 0x0f, 0x41, 0x40, 0x00, 0x80, 0x06, 0x91, 0xeb, 0x91, 0xfe, 0xa0, 0xed,
		0x41, 0xd0, 0xe4, 0xdf, 0x0d, 0x2c, 0x00, 0x50, 0x38, 0xaf, 0xfe, 0x13, 0x00, 0x00, 0x00,
		0x00, 0x70, 0x02, 0x22, 0x38, 0xc3, 0x0c, 0x00, 0x00, 0x02, 0x04, 0x05, 0xb4, 0x01, 0x01,
		0x04, 0x02
	};

	SECTION("packet including eth header (example #1)")
	{
		types::Key key1;
		types::Packet packet1;
		types::RawPacket pkt1(0, 62, pkt_data1);

		modules::RawPacketParser parser;
		std::tie(key1, packet1) = parser(pkt1);

		CHECK(key1.ip_proto == 0x06);
		CHECK(etc::format_helpers::uint32_ip_addr_to_str(key1.ip_src) == "145.254.160.237");
		CHECK(etc::format_helpers::uint32_ip_addr_to_str(key1.ip_dst) == "65.208.228.223");
		CHECK(key1.th_sport == 3372);
		CHECK(key1.th_dport == 80);

		CHECK(packet1.len == 62);
		CHECK(packet1.features.ip_ttl == 128);
		CHECK(packet1.features.tcp_flags.is_syn());
	}
}
