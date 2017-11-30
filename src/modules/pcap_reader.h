
#ifndef STARFLOW_ANALYTICS_MODULES_PCAP_READER_H
#define STARFLOW_ANALYTICS_MODULES_PCAP_READER_H

#include <stdexcept>
#include <string>

#include <pcap.h>

namespace starflow {
	namespace modules {

		class PCAPReader
		{
		public:

			enum class mode { emit = 0, store = 1 };

			explicit PCAPReader(const std::string& file_name, enum mode m = mode::emit)
				throw(std::runtime_error);

			PCAPReader()                             = delete;
			PCAPReader(const PCAPReader&)            = delete;
			PCAPReader& operator=(const PCAPReader&) = delete;
			PCAPReader(PCAPReader&&)                 = default;
			PCAPReader& operator=(PCAPReader&&)      = default;

			~PCAPReader();

		private:

			void _open(const std::string& file_name)
				throw(std::runtime_error);

			enum mode _mode;
			pcap* _pcap = nullptr;
		};
	}
}

#endif
