
#ifndef STARFLOW_FLOW_H
#define STARFLOW_FLOW_H

#include <list>
#include <chrono>

namespace starflow {
	class Flow
	{
	public:

		struct packet_t
		{
			struct features_t
			{
				features_t()                              = default;
				features_t(const features_t&)             = default;
				features_t& operator=(const features_t&)  = default;
				features_t(features_t&&)                  = default;
				features_t& operator=(features_t&&)       = default;
				~features_t()                             = default;

				unsigned ttl;
				unsigned q_len;
			};

			packet_t()                                    = delete;
			packet_t(const packet_t& copy_from)           = default;
			packet_t& operator=(const packet_t&)          = default;
			packet_t(packet_t&&)                          = default;
			packet_t& operator=(packet_t&&)               = default;

			packet_t(std::chrono::microseconds ts, unsigned size, features_t features);

			std::chrono::microseconds                     ts;
			unsigned                                      len;
			features_t                                    features;

			~packet_t()                                   = default;
		};

		Flow()                                            = default;
		Flow(const Flow& copy_from)                       = default;
		Flow& operator=(const Flow&)                      = default;
		Flow(Flow&&)                                      = default;
		Flow& operator=(Flow&&)                           = default;

		void add_packet(std::chrono::microseconds ts, unsigned len,
						packet_t::features_t features = packet_t::features_t());

		unsigned long n_packets() const;
		std::chrono::microseconds start_ts() const;
		std::chrono::microseconds recent_ts() const;

		void set_complete();
		bool complete() const;

		std::string str_desc() const;

		~Flow()                                           = default;

	private:

		std::list<packet_t>                               _packets;
		std::chrono::microseconds                         _start_ts;
		std::chrono::microseconds                         _recent_ts;
		bool                                              _complete = false;
	};
}

#endif
