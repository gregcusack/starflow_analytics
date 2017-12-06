#include "bidirectional_flow_table.h"

starflow::kernels::BiFlowTable::BiFlowTable() : _bi_flow_table() {
	input.add_port<types::CLFR>("clfr_in");
	//output.add_port<types::CLFR>("clfr_out_flow");
	//output.add_port<types::CLFR>("clfr_in_flow");
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
	_bi_flow_table.add_clfr(clfr);
	//if(_bi_flow_table.add_clfr(clfr)) {
	//	output["clfr_out_flow"].push()
	//}
	return raft::proceed;
}