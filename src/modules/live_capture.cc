
#include "live_capture.h"

starflow::modules::LiveCapture::LiveCapture(const std::string& dev_name, callback_t&& callback)
	: _dev_name(dev_name), _callback(callback)
{ }

void starflow::modules::LiveCapture::set_device_name(const std::string& dev_name)
{
	_dev_name = dev_name;
}

void starflow::modules::LiveCapture::set_callback(callback_t&& callback)
{
	_callback = callback;
}

void starflow::modules::LiveCapture::operator()() throw(std::logic_error, std::runtime_error)
{
	if (_dev_name.empty())
		throw std::logic_error("LiveCapture: no device name set");

	if (!_callback)
		throw std::logic_error("LiveCapture: no callback set");

	_connect(_dev_name);

	struct pcap_pkthdr* hdr;
	const u_char* pl;
	int rc = -1;

	while ((rc = pcap_next_ex(_pcap, &hdr, &pl)) >= 0)
		if (rc == 1)
			_callback(types::RawPacket(hdr, pl));

	if (rc < 0)
		throw std::runtime_error("LiveCapture: failed reading packet");
}

starflow::modules::LiveCapture::~LiveCapture()
{
	pcap_close(_pcap);
}

void starflow::modules::LiveCapture::_connect(const std::string& dev, int snaplen, int to_ms)
	throw(std::runtime_error)
{
	char errbuf[PCAP_ERRBUF_SIZE];

	_pcap = pcap_open_live(dev.c_str(), snaplen, 0, to_ms, errbuf);

	if(_pcap == nullptr) {
		std::string msg = "LiveCapture: could not connect to " + dev + ": " + errbuf;
		throw std::runtime_error(msg);
	}
}
