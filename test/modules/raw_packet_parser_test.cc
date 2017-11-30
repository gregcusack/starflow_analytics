
#include <catch.h>
#include <iostream>
#include "../../src/modules/raw_packet_parser.h"
#include "../../src/helpers.h"

#include "../include/raw_packet_data.h"

using namespace starflow;

TEST_CASE("RawPacketParser", "[modules::RawPacketParser]")
{
	SECTION("packet including eth header (example #1)")
	{
		types::Key key1;
		types::Packet packet1;
		types::RawPacket pkt1(0, 62, pkt_data1);

		modules::RawPacketParser parser;
		std::tie(key1, packet1) = parser(pkt1);

		REQUIRE(key1.ip_proto == 0x06);
		REQUIRE(helpers::uint32_ip_addr_to_str(key1.ip_src) == "145.254.160.237");
		REQUIRE(helpers::uint32_ip_addr_to_str(key1.ip_dst) == "65.208.228.223");
		REQUIRE(key1.th_sport == 3372);
		REQUIRE(key1.th_dport == 80);

		REQUIRE(packet1.len == 62);
		REQUIRE(packet1.features.ip_ttl == 128);
		REQUIRE(packet1.features.tcp_flags.is_syn());
	}
}
