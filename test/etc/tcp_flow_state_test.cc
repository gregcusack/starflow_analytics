
#include <catch.h>
#include "../../src/etc/tcp_flow_state.h"

using namespace starflow;

TEST_CASE("TCPFlowState", "[etc::TCPFlowState]")
{
	SECTION("TCPFlowState(): state and end are unknown on construction")
	{
		etc::TCPFlowState s;
		CHECK(s.state() == etc::TCPFlowState::state::unknown);
		CHECK(s.end()   == etc::TCPFlowState::end::unknown);
	}

	SECTION("syn()")
	{
		etc::TCPFlowState s;
		s.syn();
		CHECK(s.state() == etc::TCPFlowState::state::syn_sent);
		CHECK(s.end()   == etc::TCPFlowState::end::client);
	}

	SECTION("syn_ack()")
	{
		etc::TCPFlowState s;
		s.syn_ack();
		CHECK(s.state() == etc::TCPFlowState::state::syn_received);
		CHECK(s.end()   == etc::TCPFlowState::end::server);
	}

	SECTION("ack()")
	{
		etc::TCPFlowState s1;
		s1.syn();
		s1.ack();
		CHECK(s1.state() == etc::TCPFlowState::state::established);
		CHECK(s1.end()   == etc::TCPFlowState::end::client);

		etc::TCPFlowState s2;
		s2.syn_ack();
		s2.ack();
		CHECK(s2.state() == etc::TCPFlowState::state::established);
		CHECK(s2.end()   == etc::TCPFlowState::end::server);

		etc::TCPFlowState s3;
		s3.ack();
		CHECK(s3.state() == etc::TCPFlowState::state::established);
		CHECK(s3.end()   == etc::TCPFlowState::end::unknown);

		etc::TCPFlowState s4;
		s4.syn();
		s4.ack();
		s4.fin();
		s4.ack();
		CHECK(s4.state() == etc::TCPFlowState::state::closed);
		CHECK(s4.end()   == etc::TCPFlowState::end::client);
	}

	SECTION("fin()")
	{
		etc::TCPFlowState s1;
		s1.syn();
		s1.ack();
		s1.fin();
		CHECK(s1.state() == etc::TCPFlowState::state::fin_wait);
		CHECK(s1.end()   == etc::TCPFlowState::end::client);
	}

	SECTION("fin_ack()")
	{
		etc::TCPFlowState s1;
		s1.syn_ack();
		s1.ack();
		s1.fin_ack();
		CHECK(s1.state() == etc::TCPFlowState::state::closed);
		CHECK(s1.end()   == etc::TCPFlowState::end::server);
	}
}