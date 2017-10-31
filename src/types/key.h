
#ifndef STARFLOW_ANALYTICS_KEY_H
#define STARFLOW_ANALYTICS_KEY_H

#include <cstdint>
#include <netinet/in.h>
#include <string>

namespace starflow {
	namespace types {
		class Key
		{
		public:

			//TODO: bool operator<(Key b) const;
			//TODO:	proto::key to_proto() const;
			//TODO:	static Key from_proto(const proto::key);

			Key()                      = default;
			Key(const Key&)            = default;
			Key& operator=(const Key&) = default;
			Key(Key&&)                 = default;
			Key& operator=(Key&&)      = default;

			Key(u_char ip_p, in_addr ip_src, in_addr ip_dst, unsigned short th_sport,
				unsigned short th_dport);

			Key(std::uint8_t ip_p, std::uint32_t ip_src, std::uint32_t ip_dst,
				std::uint16_t th_sport, std::uint16_t th_dport);

			std::string str_desc() const;

			std::uint8_t  ip_proto = 0;
			std::uint32_t ip_src   = 0;
			std::uint32_t ip_dst   = 0;
			std::uint16_t th_sport = 0;
			std::uint16_t th_dport = 0;

			virtual ~Key();

		private:
			std::string _uint32_ip_addr_to_str(uint32_t addr) const;
		};
	}
}

#endif
