
#ifndef STARFLOW_KERNELS_PRINTER_H
#define STARFLOW_KERNELS_PRINTER_H

#include <iostream>

#include "../proto/starflow.pb.h"

#include "../types/raw_packet.h"


namespace starflow {

	std::ostream& operator<<(std::ostream& os, proto::packet& p)
	{
		os << "starflow::proto::packet(len=" << p.len() << ", ts=" << p.ts() << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, proto::raw_packet& p)
	{
		os << "starflow::proto::raw_packet(len=" << p.len() << ", ts=" << p.ts() << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, types::RawPacket& p)
	{
		os << "starflow::RawPacket(len=" << p.len << ", ts=" << p.ts.count() << ")";
		return os;
	}


	namespace kernels {

		template<typename T>
		class Printer : public raft::kernel
		{
		public:
			explicit Printer(std::ostream& os = std::cout, bool endl = true, bool forward = false)
				: raft::kernel(),
				  _os(os),
				  _endl(endl),
				  _forward(forward)
			{
				input.template add_port<T>("in");
				if (_forward)
					output.template add_port<T>("out");
			}

			raft::kstatus run() override
			{
				T t{};
				input["in"].pop(t);
				_os << t << (_endl ? '\n' : '\0');

				if (_forward) {
					auto out(output["out"].template allocate_s<T>());
					*out = t;
				}

				return (raft::proceed);
			}

		private:
			std::ostream& _os;
			bool _endl;
			bool _forward;
		};
	}
}

#endif
