
#ifndef STARFLOW_ANALYTICS_MODULES_PCAP_READER_H
#define STARFLOW_ANALYTICS_MODULES_PCAP_READER_H

#include <stdexcept>
#include <string>
#include <functional>
#include <list>
#include <pcap.h>

#include "../types/raw_packet.h"

namespace starflow {
	namespace modules {

		class PCAPReader
		{
		public:

			enum class mode { emit = 0, store = 1 };

			PCAPReader()                             = default;
			PCAPReader(const PCAPReader&)            = delete;
			PCAPReader& operator=(const PCAPReader&) = delete;
			PCAPReader(PCAPReader&&)                 = default;
			PCAPReader& operator=(PCAPReader&&)      = default;

			void set_file_name(const std::string& file_name);

			void set_mode(mode mode);

			void set_store_mode();

			void set_emit_mode();

			void set_callback(std::function<void (types::RawPacket)> callback);

//          TODO: not yet implemented
//			void set_playback(bool playback);

			const PCAPReader& operator()() throw(std::runtime_error, std::logic_error);

			const std::list<types::RawPacket>& packets() const;

			~PCAPReader() = default;

		private:

			void _open(const std::string& file_name)
				throw(std::runtime_error);

			void _close();

			std::string _file_name = "";
			enum mode _mode = mode::emit;
			std::function<void (types::RawPacket)> _callback = nullptr;
			bool _playback = true;
			pcap* _pcap = nullptr;
			std::list<types::RawPacket> _packets = {};
		};
	}
}

#endif
