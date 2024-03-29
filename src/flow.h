
#ifndef STARFLOW_FLOW_H
#define STARFLOW_FLOW_H

#include <list>
#include <chrono>

#include "proto/starflow.pb.h"

namespace starflow {

	class Flow
	{
	public:

		struct packet_t
		{
			struct features_t
			{
				features_t() : ttl(0), q_len(0) { }
				features_t(const features_t&)             = default;
				features_t& operator=(const features_t&)  = default;
				features_t(features_t&&)                  = default;
				features_t& operator=(features_t&&)       = default;
				~features_t()                             = default;

				unsigned ttl = 0;
				unsigned q_len = 0;

				proto::features to_proto() const;
				static features_t from_proto(const proto::features&);
			};

			packet_t()                                    = default;
			packet_t(const packet_t& copy_from)           = default;
			packet_t& operator=(const packet_t&)          = default;
			packet_t(packet_t&&)                          = default;
			packet_t& operator=(packet_t&&)               = default;

			packet_t(std::chrono::microseconds ts, unsigned size, features_t features);

			std::chrono::microseconds                     ts = std::chrono::microseconds(0);
			unsigned                                      len = 0;
			features_t                                    features = features_t();

			proto::packet to_proto() const;
			static packet_t from_proto(const proto::packet&);

			~packet_t()                                   = default;
		};

		Flow()                                            = default;
		Flow(const Flow& copy_from)                       = default;
		Flow& operator=(const Flow&)                      = default;
		Flow(Flow&&)                                      = default;
		Flow& operator=(Flow&&)                           = default;

		void add_packet(std::chrono::microseconds ts, unsigned len,
						packet_t::features_t features = packet_t::features_t());

		const std::list<packet_t> packets() const;

		unsigned long n_packets() const;
		std::chrono::microseconds start_ts() const;
		std::chrono::microseconds recent_ts() const;

		void set_eviction_ts(std::chrono::microseconds ts);
		std::chrono::microseconds eviction_ts() const;

		void set_complete();
		bool complete() const;

		std::string str_desc() const;

		proto::flow to_proto() const;
		static Flow from_proto(const proto::flow&);

		~Flow()                                           = default;

	private:

		std::list<packet_t>                               _packets = {};
		std::chrono::microseconds                         _start_ts = std::chrono::microseconds(0);
		std::chrono::microseconds                         _recent_ts = std::chrono::microseconds(0);
		std::chrono::microseconds                         _eviction_ts = std::chrono::microseconds(0);
		bool                                              _complete = false;
	};
}

#endif
