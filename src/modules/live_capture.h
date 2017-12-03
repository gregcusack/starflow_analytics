
#ifndef STARFLOW_ANALYTICS_LIVE_CAPTURE_H
#define STARFLOW_ANALYTICS_LIVE_CAPTURE_H

#include <pcap.h>

#include <string>
#include <functional>
#include "../types/raw_packet.h"

namespace starflow {
	namespace modules {

		class LiveCapture
		{
			using callback_t = std::function<void (types::RawPacket)>;

		public:
			LiveCapture() = default;
			explicit LiveCapture(const std::string& dev_name, callback_t&& = nullptr);

			LiveCapture(const LiveCapture&)       = delete;
			LiveCapture& operator=(LiveCapture&)  = delete;
			LiveCapture(LiveCapture&&)            = default;
			LiveCapture& operator=(LiveCapture&&) = default;

			void set_device_name(const std::string& dev_name);
			void set_callback(callback_t&& callback);

			void operator()() throw (std::logic_error, std::runtime_error);

			~LiveCapture();

		private:
			void _connect(const std::string& dev, int snaplen = 1600, int to_ms = 500)
				throw (std::runtime_error);

			std::string _dev_name = "";
			pcap_t* _pcap         = nullptr;
			callback_t _callback  = nullptr;
		};
	}
}

#endif
