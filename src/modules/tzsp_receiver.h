
#ifndef STARFLOW_ANALYTICS_MODULES_TZSP_RECEIVER_H
#define STARFLOW_ANALYTICS_MODULES_TZSP_RECEIVER_H

#include <cstdint>
#include <stdexcept>
#include <functional>

#include "../types/raw_packet.h"

namespace starflow {
	namespace modules {

		class TZSPReceiver
		{
			using callback_t = std::function<void (types::RawPacket)>;

			struct tzsp_header {
				uint8_t  version;
				uint8_t  type;
				uint16_t encap;
			};

			struct tzsp_tag {
				uint8_t type;
				uint8_t length;
				char    data[];
			};

		public:
			TZSPReceiver();
			explicit TZSPReceiver(unsigned short listen_port, callback_t&& callback = nullptr);

			void set_listen_port(unsigned short listen_port);
			void set_callback(callback_t&& callback);
			void operator()() throw (std::runtime_error, std::logic_error);

			~TZSPReceiver();

		private:
			int _sock                   = -1;
			unsigned short _listen_port = 0;
			callback_t _callback        = nullptr;
		};
	}
}

#endif
