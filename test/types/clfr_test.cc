
#include <catch.h>
#include "../../src/types/clfr.h"

using namespace starflow;

TEST_CASE("CLFR", "[types::CLFR]")
{
	types::Key k1(0x6, 4393423, 4324893, 2392, 24933);
	types::Key k2(0x6, 4304042, 3483999, 3503, 12882);

	SECTION("CLFR(): can be constructed without a key")
	{
		types::CLFR clfr;
		CHECK(clfr.key().ip_proto == 0);
		CHECK(clfr.key().ip_src   == 0);
		CHECK(clfr.key().ip_dst   == 0);
		CHECK(clfr.key().th_sport == 0);
		CHECK(clfr.key().th_dport == 0);
	}

	SECTION("CLFR(): can be constructed with a key")
	{
		types::CLFR clfr(k1);
		CHECK(clfr.key() == k1);
	}
}
