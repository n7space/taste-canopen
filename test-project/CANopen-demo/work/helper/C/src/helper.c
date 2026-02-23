#include "helper.h"
#include "dataview-uniq.h"

#include <assert.h>

void helper_startup(void) {}

void helper_PI_do_init(void) {
  asn1SccCANopen_Init_Result result;
  helper_RI_init(&result);
  assert(result == asn1SccCANopen_Init_Result_success &&
         "CANopen node initialization failed!");
}

void helper_PI_do_reset(void) { helper_RI_reset_node(); }

void helper_PI_change_node_state(const asn1SccCANopen_NMT_State *state) {
  helper_RI_change_node_state(state);
}

void helper_PI_issue_slave_command(const asn1SccNode_Command_Request *request) {
  helper_RI_issue_slave_command(&request->node_id, &request->command);
}

void helper_PI_objdict_get(const asn1SccGet_Data_Request *request) {
  asn1SccCANopen_Value value;
  asn1SccCANopen_ObjDict_Operation_Result result;

  helper_RI_get_object_dictionary_data(&request->object, &request->subobject,
                                       &value, &result);

  assert(result == asn1SccCANopen_ObjDict_Operation_Result_success &&
         "Object dictionary read failed!");

  const asn1SccGet_Data_Response response = {.object = request->object,
                                             .subobject = request->subobject,
                                             .data_value = value};

  helper_RI_objdict_get_result(&response);
}

void helper_PI_objdict_set(const asn1SccSet_Data_Request *request) {
  asn1SccCANopen_ObjDict_Operation_Result result;
  helper_RI_set_object_dictionary_data(&request->object, &request->subobject,
                                       &request->data_value, &result);

  assert(result == asn1SccCANopen_ObjDict_Operation_Result_success &&
         "Object dictionary write failed!");
}
