#include "bidirectional_flow_table.h"

starflow::kernels::BiFlowTable::BiFlowTable() {
	input.add_port<types::CLFR>("clfr_in");
	output.add_port<types::CLFR>("clfr_out_flow");
	output.add_port<types::CLFR>("clfr_in_flow");
/*
	_bi_flow_table.set_mode(modules::BiFlowTable::mode::callback);
	_bi_flow_table.set_callback([this](types::CLFR f) {
		output["clfr_out"].push(f);
	});
*/
}

raft::kstatus starflow::kernels::BiFlowTable::run() {
	types::CLFR clfr;
	input["clfr_in"].pop(clfr);
	starflow::kernels::BiFlowTable::bi_flow_table_t::iterator k;
	//std::map<types::Key, std::pair<types::CLFR, types::CLFR>>::iterator k;

	k = add_clfr(clfr);
	bool flag = false;
	if (k != _active_flows.end()) {
		flag = true;
		output["clfr_out_flow"].push(k->second.first);
		output["clfr_in_flow"].push(k->second.second);
	}
	return raft::proceed;
}

starflow::kernels::BiFlowTable::bi_flow_table_t::iterator
 starflow::kernels::BiFlowTable::add_clfr(types::CLFR clfr)
	throw(std::logic_error)
{
	types::Key ikey = clfr.key();
	types::Key nkey = clfr.key();
	bool flag = false;
	bool src_ip = 	(ikey.get_ip_src().find("10.") != std::string::npos) ||
			   		(ikey.get_ip_src().find("172.16") != std::string::npos) ||
			   		(ikey.get_ip_src().find("172.31") != std::string::npos) ||
			   		(ikey.get_ip_src().find("192.168.") != std::string::npos);
	bi_flow_table_t::iterator k;
	for(k = _active_flows.begin(); k != _active_flows.end(); k++) {
		if(ikey.ip_proto != k->first.ip_proto) {
			//std::cout << "protos dif" << std::endl;
			continue;
		}
		if(!((ikey.ip_src == k->first.ip_dst && ikey.th_sport == k->first.th_dport)  && 
			(ikey.ip_dst == k->first.ip_src && ikey.th_dport == k->first.th_sport))) {
				//std::cout << "no match" << std::endl;
				continue;
		}
		else {
			if(src_ip) {
				k->second.first = clfr;
				//std::cout << "outgoing" << std::endl;
			}
			else{
				k->second.second = clfr;
				//std::cout << "incoming!" << std::endl;
			}
			return k;
		}
	}
	//if(!flag) {
	k = _active_flows.emplace(ikey, std::pair<types::CLFR, types::CLFR>(clfr, clfr)).first;
	k = _active_flows.end();
	return k;
}