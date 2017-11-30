
#include <catch.h>

#include "../../src/modules/pcap_reader.h"

using namespace starflow::modules;


TEST_CASE("PCAPReader()", "[modules::PCAPReader]")
{
	SECTION("throws an exception if the file cannot be opened")
	{
		REQUIRE_NOTHROW(PCAPReader("test/data/http.pcap"));
		REQUIRE_THROWS(PCAPReader("test/data/doesnotexist.pcap"));
	}
}
