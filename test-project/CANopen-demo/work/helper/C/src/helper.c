#include "helper.h"
#include "dataview-uniq.h"

void helper_startup(void) {}

void helper_PI_do_init(void) { helper_RI_init(); }

void helper_PI_do_reset(void) { helper_RI_reset_node(); }

void helper_PI_change_node_state(const asn1SccCANopen_NMT_State *state) {
  helper_RI_change_node_state(state);
}

void helper_PI_issue_slave_command(const asn1SccNode_Command_Request *request) {
  helper_RI_issue_slave_command(&request->node_id, &request->command);
}

void helper_PI_objdict_get(const asn1SccGet_Data_Request *request) {
  asn1SccCANopen_Value value;

  helper_RI_get_object_dictionary_data(&request->object, &request->subobject,
                                       &value);

  const asn1SccGet_Data_Response response = {.object = request->object,
                                             .subobject = request->subobject,
                                             .data_value = value};

  helper_RI_objdict_get_result(&response);
}

void helper_PI_objdict_set(const asn1SccSet_Data_Request *request) {
  helper_RI_set_object_dictionary_data(&request->object, &request->subobject,
                                       &request->data_value);
}
