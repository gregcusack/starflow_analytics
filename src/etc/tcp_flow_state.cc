
#include "tcp_flow_state.h"

void starflow::etc::TCPFlowState::syn()
{
	switch (_state) {
		case state::unknown:
			_state = state::syn_sent;
			_end   = end::client;
			break;

		default: break;
	}
}

void starflow::etc::TCPFlowState::syn_ack()
{
	switch (_state) {
		case state::unknown:
			_state = state::syn_received;
			_end   = end::server;
			break;

		default: break;
	}
}

void starflow::etc::TCPFlowState::ack()
{
	switch (_state) {
		case state::unknown:
			_state = state::established;
			_end   = end::unknown;
			break;

		case state::syn_sent:
			_state = state::established;
			_end   = end::client;
			break;

		case state::syn_received:
			_state = state::established;
			_end   = end::server;
			break;

		case state::established:
			_state = state::established;
			break;

		case state::fin_wait:
			_state = state::closed;
			break;

		default: break;
	}
}

void starflow::etc::TCPFlowState::fin()
{
	switch (_state) {
		case state::established:
			_state = state::fin_wait;
			break;

		default: break;
	}
}

void starflow::etc::TCPFlowState::fin_ack()
{
	switch (_state) {
		case state::established:
			_state = state::closed;
			break;

		default: break;
	}
}
