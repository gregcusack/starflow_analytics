
#include <catch.h>
#include <iomanip>
#include <iostream>
#include "../../src/modules/pcap_reader.h"

using namespace starflow;

TEST_CASE("PCAPReader", "[PCAPReader]")
{

	SECTION("operator(): throws a logic error if no filename is set")
	{
		modules::PCAPReader r;
		r.set_callback([](types::RawPacket p) { });
		CHECK_THROWS_AS(r(), std::logic_error);
	}

	SECTION("operator():throws a logic error if in emit mode and no callback set")
	{
		modules::PCAPReader r;
		r.set_file_name("test/data/http.pcap");
		CHECK_THROWS_AS(r(), std::logic_error);
	}

	SECTION("operator():does not throw an error if in store mode and no callback set")
	{
		modules::PCAPReader r;
		r.set_file_name("test/data/http.pcap");
		r.set_mode(modules::PCAPReader::mode::store);
		CHECK_NOTHROW(r());
	}

	SECTION("operator():throws a runtime error if the file cannot be opened")
	{
		modules::PCAPReader r;
		r.set_file_name("test/data/doesnotexist.pcap");
		r.set_callback([](types::RawPacket p) { });
		CHECK_THROWS_AS(r(), std::runtime_error);
	}

	SECTION("operator():stores packets to packet list if in store mode")
	{
		modules::PCAPReader r;
		r.set_mode(modules::PCAPReader::mode::store);
		r.set_file_name("test/data/http.pcap");

		r();

		CHECK(r.packets().size() == 43);

		auto i = std::begin(r.packets());

		CHECK(i->len == 62);
		i++;
		CHECK(i->len == 62);
		i++;
		CHECK(i->len == 54);
		i++;
		CHECK(i->len == 533);

		CHECK(r.packets().back().len == 54);
	}
}
