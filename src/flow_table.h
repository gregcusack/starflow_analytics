

#ifndef STARFLOW_FLOW_TABLE_H
#define STARFLOW_FLOW_TABLE_H

#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <iostream>
#include <map>
#include <functional>

#include "flow.h"

#include <string>
#include <unordered_map>

namespace starflow {

	class FlowTable
	{
	public:

		enum class outer_header_type {
			eth = 0,
			ip  = 1
		};

		enum class eviction_type {
			udp_to  = 0,
			tcp_fin = 1,
			tcp_to  = 2
		};

		class key_t {
		public:
			std::uint8_t  ip_proto;
			std::uint32_t ip_src;
			std::uint32_t ip_dst;
			std::uint16_t th_sport;
			std::uint16_t th_dport;

			key_t() = default;
			key_t(const key_t&) = default;

			key_t(u_char ip_p, in_addr ip_src, in_addr ip_dst, unsigned short th_sport,
				  unsigned short th_dport);

			key_t(std::uint8_t, std::uint32_t ip_src, std::uint32_t ip_dst, std::uint16_t th_sport,
				  std::uint16_t th_dport);

			std::string str_desc() const;

			bool operator<(key_t b) const;
		};

		FlowTable()                            = delete;
		FlowTable(const FlowTable&)            = default;
		FlowTable& operator=(const FlowTable&) = default;
		FlowTable(FlowTable&&)                 = default;
		FlowTable& operator=(FlowTable&&)      = default;
		~FlowTable()                           = default;

		explicit FlowTable(
			std::function<void (key_t, Flow, std::chrono::microseconds, eviction_type)> f,
			std::chrono::seconds timeout_check_period = std::chrono::seconds(1),
			std::chrono::seconds tcp_timeout = std::chrono::seconds(300),
			std::chrono::seconds udp_timeout = std::chrono::seconds(30));

		void add_packet(std::chrono::microseconds ts, unsigned len, const unsigned char* buf,
						outer_header_type outer = outer_header_type::eth);

		void add_packet(std::chrono::microseconds ts, unsigned len, key_t key, bool fin = false);

		static std::string uint32_ip_addr_to_str(uint32_t addr);

		static std::string eviction_type_to_string(eviction_type e);

	private:

		void _evict_flow(key_t key, std::chrono::microseconds ts, eviction_type e);

		void _check_flow_timeouts(std::chrono::microseconds ts);

		std::map<key_t, Flow>             _flows;
		std::function<void (key_t, Flow, std::chrono::microseconds, eviction_type)> _evict_handler;
		std::chrono::microseconds         _last_timeout_check;
		std::chrono::seconds              _timeout_check_period = std::chrono::seconds(1);
		std::chrono::seconds              _udp_timeout          = std::chrono::seconds(30);
		std::chrono::seconds              _tcp_timeout          = std::chrono::seconds(30);
	};
}

#endif
