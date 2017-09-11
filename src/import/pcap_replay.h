
#ifndef STARFLOW_IMPORT_PCAP_REPLAY_H
#define STARFLOW_IMPORT_PCAP_REPLAY_H

#include <chrono>
#include <string>
#include <functional>
#include <pcap/pcap.h>

namespace starflow {
	namespace import {

		class PCAPReplay
		{
		public:
			PCAPReplay()                             = delete;
			PCAPReplay(const PCAPReplay&)            = default;
			PCAPReplay& operator=(const PCAPReplay&) = default;
			PCAPReplay(PCAPReplay&&)                 = default;
			PCAPReplay& operator=(PCAPReplay&&)      = default;

			explicit PCAPReplay(const std::string& file_name, bool sleep = true)
				throw(std::runtime_error);

			void operator()(std::function<void (struct pcap_pkthdr*, const unsigned char*)> f);

			~PCAPReplay();

			static std::chrono::microseconds us_from_timeval(timeval& t);

		private:
			unsigned long _i                   = 0;
			pcap_t* _pcap                      = nullptr;
			std::chrono::microseconds _last_ts = std::chrono::microseconds();
			bool _sleep;
		};
	}
}

#endif
