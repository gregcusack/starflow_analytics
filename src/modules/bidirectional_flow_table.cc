#include "bidirectional_flow_table.h"

void starflow::modules::BiFlowTable::add_clfr(types::CLFR clfr)
	throw(std::logic_error)
{
	//if(_mode == mode::callback && !_callback)
	//	throw std::logic_error("FlowTable: no callback function set");

	types::Key ikey = clfr.key();
	types::Key nkey = clfr.key();
	//std::cout << ikey.get_ip_src() << std::endl;
	bool flag = false;
	bool src_ip = 	(ikey.get_ip_src().find("10.") != std::string::npos) ||
			   		(ikey.get_ip_src().find("172.16") != std::string::npos) ||
			   		(ikey.get_ip_src().find("172.31") != std::string::npos) ||
			   		(ikey.get_ip_src().find("192.168.") != std::string::npos);

	//std::map<types::Key, std::pair<types::CLFR, types::CLFR>>::iterator k;
	//k = _active_flows.begin();
	//auto k = _active_flows.begin();
	bi_flow_table_t::iterator k;
	for(k = _active_flows.begin(); k != _active_flows.end(); k++) {
		if(ikey.ip_proto != k->first.ip_proto) {
			//std::cout << "protos dif" << std::endl;
			continue;
		}
		if(!(ikey.ip_src == k->first.ip_dst && ikey.th_sport == k->first.th_dport)  && 
			(ikey.ip_dst == k->first.ip_src && ikey.th_dport == k->first.th_sport)) {
				//std::cout << "no match" << std::endl;
				continue;
		}
		else {
			if(src_ip) {
				k->second.first = clfr;
				std::cout << "outgoing" << std::endl;
			}
			else{
				k->second.second = clfr;
				std::cout << "incoming!" << std::endl;
			}
			//_evict_flow(
			flag = true;
			break;
			//return true;
		}
	}
	if(!flag) {
		k = _active_flows.emplace(ikey, std::pair<types::CLFR, types::CLFR>(clfr, clfr)).first;
	}
	//if(!flag) { //key not in bi_flow_table
	//	if(src_ip) { //set as both --> idk how to get around this
	//		_active_flows.emplace(ikey, std::pair<types::CLFR, types::CLFR>(clfr, clfr));
	//	}
	//}

}
