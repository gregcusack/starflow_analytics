
#include <catch.h>
#include "../../src/types/raw_packet.h"

using namespace starflow;

TEST_CASE("RawPacket", "[types::RawPacket]")
{
	unsigned char test_data1[] = "Djkqemdi39432Je2d2J934mF"; //24
	unsigned char test_data2[] = "HJSjh933jdfSdhl37elSDfjd"; //24

	SECTION("RawPacket(): can be constructed using an unsigned char array")
	{
		types::RawPacket p1(717, 24, test_data1);
		CHECK(p1.ts     == std::chrono::microseconds(717));
		CHECK(p1.len    == 24);
		CHECK(p1.pl[0]  == 'D');
		CHECK(p1.pl[10] == '4');
		CHECK(p1.pl[23] == 'F');
	}

	SECTION("RawPacket(RawPacket&): can be copy-constructed from another RawPacket")
	{
		types::RawPacket p1(717, 24, test_data1);
		types::RawPacket p2(p1);
		CHECK(p2.ts     == p1.ts);
		CHECK(p2.len    == p1.len);
		CHECK(p2.pl[0]  == p1.pl[0]);
		CHECK(p2.pl[10] == p1.pl[10]);
		CHECK(p2.pl[23] == p1.pl[23]);
	}

	SECTION("operator=(RawPacket&): can be copy-assigned from another RawPacket")
	{
		types::RawPacket p1(717, 24, test_data1);
		types::RawPacket p2 = p1;
		CHECK(p2.ts     == p1.ts);
		CHECK(p2.len    == p1.len);
		CHECK(p2.pl[0]  == p1.pl[0]);
		CHECK(p2.pl[10] == p1.pl[10]);
		CHECK(p2.pl[23] == p1.pl[23]);
	}

	SECTION("RawPacket(RawPacket&&): can be moved")
	{
		types::RawPacket p1(717, 24, test_data1);
		types::RawPacket p2 = std::move(p1);

		CHECK(p2.ts     == std::chrono::microseconds(717));
		CHECK(p2.len    == 24);
		CHECK(p2.pl[0]  == 'D');
		CHECK(p2.pl[10] == '4');
		CHECK(p2.pl[23] == 'F');
	}
}
