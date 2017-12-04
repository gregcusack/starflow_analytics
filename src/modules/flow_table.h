
#ifndef STARFLOW_ANALYTICS_MODULES_FLOW_TABLE_H
#define STARFLOW_ANALYTICS_MODULES_FLOW_TABLE_H

#include "../types/key.h"
#include "../types/clfr.h"

#include <map>
#include <stdexcept>

namespace starflow {
	namespace modules {

		class FlowTable
		{
			using flow_table_t           = std::map<types::Key, types::CLFR>;
			using exported_flows_table_t = std::list<std::pair<types::Key, types::CLFR>>;
			using export_flow_callback_t = std::function<void (types::CLFR)>;

		public:

			enum class incomplete_evict_policy { none, to, pkt_count };
			enum class mode { callback, store };

			void add_packet(types::Key key, types::Packet packet)
				throw(std::logic_error);

			void add_packet(std::pair<types::Key, types::Packet> pair)
				throw(std::logic_error);

			void set_mode(mode m);

			void set_callback(export_flow_callback_t&& callback);

			unsigned long long count_packets_processed() const;
			unsigned long long count_flows_processed() const;
			unsigned long long count_packets() const;
			unsigned long long count_flows() const;

			const exported_flows_table_t& exported_flows() const;

			void force_export_tcp(bool complete = false);
			void force_export_udp(bool complete = false);

		private:
			flow_table_t _active_flows                       = {};
			exported_flows_table_t _exported_flows           = {};

			std::chrono::microseconds _to_check_interval     = std::chrono::seconds(10);
			std::chrono::microseconds _udp_to                = std::chrono::seconds(30);
			std::chrono::microseconds _last_to_check         = std::chrono::seconds(0);

			FlowTable::mode _mode                            = mode::callback;

			incomplete_evict_policy _incomplete_evict_policy = incomplete_evict_policy::none;
			std::chrono::microseconds _incomplete_evict_to   = std::chrono::seconds(10);
			unsigned long _incomplete_evict_pkt_count        = 50;

			unsigned long long _n_packets_processed          = 0;
			unsigned long long _n_flows_processed            = 0;
			unsigned long long _n_packets                    = 0;
			unsigned long long _n_flows                      = 0;

			export_flow_callback_t _callback = nullptr;

			void _check_timeouts(std::chrono::microseconds trigger_ts);

			flow_table_t::iterator _evict_flow(const flow_table_t::iterator& i,
											   std::chrono::microseconds evict_ts, bool complete);
		};
	}
}

#endif
