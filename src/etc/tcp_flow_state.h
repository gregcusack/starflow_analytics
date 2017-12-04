
#ifndef STARFLOW_ANALYTICS_ETC_TCP_FLOW_STATE_H
#define STARFLOW_ANALYTICS_ETC_TCP_FLOW_STATE_H

namespace starflow {
	namespace etc {

		class TCPFlowState
		{
		public:

			enum class state
			{
				unknown, closed, syn_received, syn_sent, established, fin_wait
			};

			enum class end
			{
				client, server, unknown
			};

			TCPFlowState() = default;
			inline const enum state& state() { return _state; }
			inline const enum end& end() { return _end; }

			void syn();
			void syn_ack();
			void ack();
			void fin();
			void fin_ack();

		private:
			enum state _state = state::unknown;
			enum end   _end   = end::unknown;
		};
	}
}

#endif