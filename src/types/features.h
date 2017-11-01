
#ifndef STARFLOW_ANALYTICS_FEATURES_H
#define STARFLOW_ANALYTICS_FEATURES_H

#include <string>

namespace starflow {
	namespace types {
		class Features
		{
		public:

			class tcp_flags_t
			{
			public:
				tcp_flags_t();
				explicit tcp_flags_t(unsigned char flags);
				tcp_flags_t(const tcp_flags_t&)            = default;
				tcp_flags_t& operator=(const tcp_flags_t&) = default;
				tcp_flags_t(tcp_flags_t&&)                 = default;
				tcp_flags_t& operator=(tcp_flags_t&&)      = default;
				inline bool is_urg() const { return (_flags & _urg) == _urg; }
				inline bool is_ack() const { return (_flags & _ack) == _ack; }
				inline bool is_psh() const { return (_flags & _psh) == _psh; }
				inline bool is_rst() const { return (_flags & _rst) == _rst; }
				inline bool is_syn() const { return (_flags & _syn) == _syn; }
				inline bool is_fin() const { return (_flags & _fin) == _fin; }
				std::string str_desc() const;

			private:
				unsigned char _flags;
				unsigned char _urg = 32;
				unsigned char _ack = 16;
				unsigned char _psh =  8;
				unsigned char _rst =  4;
				unsigned char _syn =  2;
				unsigned char _fin =  1;
			};

			//TODO: bool operator<(Features b) const;
			//TODO:	proto::features to_proto() const;
			//TODO:	static Features from_proto(const proto::features);

			Features()                           = default;
			Features(const Features&)            = default;
			Features& operator=(const Features&) = default;
			Features(Features&&)                 = default;
			Features& operator=(Features&&)      = default;

			//TODO: look at std::optional spec;
			unsigned char ip_ttl = 0;
			tcp_flags_t tcp_flags = tcp_flags_t(0);

			virtual ~Features();
		};
	}
}


#endif
