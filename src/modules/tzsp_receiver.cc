
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "tzsp_receiver.h"

starflow::modules::TZSPReceiver::TZSPReceiver()
	: _sock(socket(AF_INET, SOCK_DGRAM, 0))
{
	if (_sock < 0)
		throw std::runtime_error("TZSPReceiver: could not open socket");
}

starflow::modules::TZSPReceiver::TZSPReceiver(unsigned short listen_port, callback_t&& callback)
	: _sock(socket(AF_INET, SOCK_DGRAM, 0)),
	  _listen_port(listen_port),
	  _callback(std::move(callback)) { }

void starflow::modules::TZSPReceiver::set_listen_port(unsigned short listen_port)
{
	_listen_port = listen_port;
}

void starflow::modules::TZSPReceiver::set_callback(callback_t&& callback)
{
	_callback = std::move(callback);
}

void starflow::modules::TZSPReceiver::operator()() throw (std::runtime_error, std::logic_error)
{
	if (!_callback)
		throw std::logic_error("TZSPReceiver: callback is not set");

	if (!_listen_port)
		throw std::logic_error("TZSPReceiver: listen port ist not set");

	char buf[1800];
	char* msg_ptr;
	char* msg_end;
	ssize_t bytes = 0;

	struct sockaddr from_addr = {};
	socklen_t from_len = sizeof(from_addr);

	struct sockaddr_in in_addr = {
		.sin_family = AF_INET,
		.sin_addr = { .s_addr = htonl(INADDR_ANY) },
		.sin_port = _listen_port
	};

	if (bind(_sock, (struct sockaddr*) &in_addr, sizeof(in_addr)))
		throw std::runtime_error("TZSPReceiver: could not bind socket");

	while ((bytes = recvfrom(_sock, buf, 1800, 0, &from_addr, &from_len)) > 0) {

		msg_end = buf + bytes;
		auto tzsp_hdr = (tzsp_header*) buf;
		msg_ptr = buf + sizeof(struct tzsp_header);


		if (tzsp_hdr->version == 1 && tzsp_hdr->type == 0) { // type 0: TZSP_TYPE_RECEIVED_TAG_LIST

			auto ts = std::chrono::system_clock::now();

			while (msg_ptr < msg_end) {

				auto tag = (struct tzsp_tag *) buf;

				if (tag->type == 1) { // tag end
					msg_ptr++;
					break;
				}
				else if (tag->type == 0) { //tag padding
					msg_ptr++;
				}
				else {
					if (msg_ptr + sizeof(struct tzsp_tag) > msg_end ||
						msg_ptr + sizeof(struct tzsp_tag) + tag->length > msg_end)
					{
						// malformed packet - skip
						break;
					}
					msg_ptr += sizeof(struct tzsp_tag) + tag->length;
				}
			}

			auto ts_us = (unsigned long) std::chrono::microseconds(
				std::chrono::duration_cast<std::chrono::microseconds>(
					ts.time_since_epoch()).count()).count();

			auto len = (unsigned) (msg_end - msg_ptr);

			_callback(types::RawPacket(ts_us, len, (const unsigned char*) msg_ptr));
		}
	}
}

starflow::modules::TZSPReceiver::~TZSPReceiver()
{
	close(_sock);
}
