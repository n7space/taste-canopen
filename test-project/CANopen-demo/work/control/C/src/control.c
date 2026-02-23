#include "control.h"
#include "dataview-uniq.h"
#include <stdbool.h>
#include <stdio.h>

static uint32_t ticks = 0;
static const int16_t startupODValue = 0x1234;
static int16_t odValue = 0x1234;
static bool demoFinished = false;

// Helper function to convert Init_Result enum to string
static const char *init_result_to_string(asn1SccCANopen_Init_Result result) {
  switch (result) {
  case CANopen_Init_Result_success:
    return "success";
  case CANopen_Init_Result_already_initialized:
    return "already_initialized";
  case CANopen_Init_Result_device_creation_failure:
    return "device_creation_failure";
  case CANopen_Init_Result_network_creation_failure:
    return "network_creation_failure";
  case CANopen_Init_Result_nmt_creation_failure:
    return "nmt_creation_failure";
  case CANopen_Init_Result_sync_creation_failure:
    return "sync_creation_failure";
  default:
    return "unknown";
  }
}

// Helper function to convert ObjDict_Operation_Result enum to string
static const char *objdict_operation_result_to_string(
    asn1SccCANopen_ObjDict_Operation_Result result) {
  switch (result) {
  case CANopen_ObjDict_Operation_Result_success:
    return "success";
  case CANopen_ObjDict_Operation_Result_device_not_initialized:
    return "device_not_initialized";
  case CANopen_ObjDict_Operation_Result_invalid_index:
    return "invalid_index";
  case CANopen_ObjDict_Operation_Result_invalid_subindex:
    return "invalid_subindex";
  case CANopen_ObjDict_Operation_Result_unsupported_type:
    return "unsupported_type";
  case CANopen_ObjDict_Operation_Result_write_failed:
    return "write_failed";
  default:
    return "unknown";
  }
}

// Helper function to convert NMT_Command enum to string (used by
// slave_command_result_to_string)
static const char *nmt_command_to_string(asn1SccCANopen_NMT_Command command) {
  switch (command) {
  case CANopen_NMT_Command_start:
    return "start";
  case CANopen_NMT_Command_stop:
    return "stop";
  case CANopen_NMT_Command_enter_preop:
    return "enter_preop";
  case CANopen_NMT_Command_reset_node:
    return "reset_node";
  case CANopen_NMT_Command_reset_comm:
    return "reset_comm";
  default:
    return "unknown";
  }
}

void control_startup(void) {}

void control_PI_trigger(void) {
  static bool initTriggered = false;
  static bool nodeResetTriggered = false;
  static bool stateChangeTriggered = false;
  static bool slaveChangeTriggered = false;

  const uint16_t odObjectIndex = 0x2002;
  const uint8_t odObjectSubIndex = 0x0;

  if (demoFinished)
    return;

  if (!initTriggered) {
    printf("[control] Triggering initialization...\n");
    control_RI_do_init();
    initTriggered = true;
  }

  if (ticks == 1 && !nodeResetTriggered) {
    printf("[control] Triggering node reset...\n");
    control_RI_do_reset();
    nodeResetTriggered = true;
  }

  if (ticks > 2 && ticks % 2 == 0) {
    printf("[control] Requesting OD value @ %04X:%02X...\n", odObjectIndex,
           odObjectSubIndex);
    const asn1SccGet_Data_Request request = {.object = odObjectIndex,
                                             .subobject = odObjectSubIndex};
    control_RI_objdict_get(&request);
  }

  if (ticks > 2 && ticks % 2 == 1) {
    printf("[control] Setting OD value @ %04X:%02X = 0x%04X...\n",
           odObjectIndex, odObjectSubIndex, odValue);
    const asn1SccSet_Data_Request request = {
        .object = odObjectIndex,
        .subobject = odObjectSubIndex,
        .data_value = {.kind = CANopen_Value_signed16_PRESENT,
                       .u = {.signed16 = odValue}}};
    control_RI_objdict_set(&request);

    odValue++;
  }

  if (ticks == 3 && !slaveChangeTriggered) {
    printf("[control] Triggering slave node command (may fail when other node "
           "is not present)...\n");
    const asn1SccNode_Command_Request request = {
        .node_id = 10, .command = asn1SccCANopen_NMT_Command_reset_node};
    control_RI_issue_slave_command(&request);
    slaveChangeTriggered = true;
  }

  if (ticks == 5 && !stateChangeTriggered) {
    printf("[control] Triggering state change (stopping current node)...\n");
    const asn1SccCANopen_NMT_State newState = CANopen_NMT_State_stop;
    control_RI_change_node_state(&newState);
    stateChangeTriggered = true;
  }

  if (ticks > 10) {
    printf("[control] DEMO SUCCESSFUL! Switching to idle state...\n");
    demoFinished = true;
  }

  ticks++;
}

void control_PI_init_result(const asn1SccCANopen_Init_Result *result) {
  printf("[control] Initialization result: %s\n",
         init_result_to_string(*result));

  if (*result != CANopen_Init_Result_success) {
    printf(
        "[control] !!! DEMO FAILED DUE TO CANOPEN INITIALIZATION ISSUE !!!\n");
    demoFinished = true;
  }
}

void control_PI_objdict_get_result(const asn1SccGet_Data_Response *response) {
  if (response->result != CANopen_ObjDict_Operation_Result_success) {
    printf("[control] !!! DEMO FAILED !!! Getting object dictionary value has "
           "failed due to: %s\n",
           objdict_operation_result_to_string(response->result));
    demoFinished = true;
  }

  const uint16_t expectedValue = startupODValue + ((ticks - 4) / 2);
  printf("[control] Received object dictionary value, expecting: 0x%04X, got: "
         "%04lX:%02lX = ",
         expectedValue, response->object, response->subobject);

  switch (response->data_value.kind) {
  case CANopen_Value_boolean8_PRESENT:
    printf("0x%s\n", response->data_value.u.boolean8 ? "true" : "false");
    break;
  case CANopen_Value_unsigned8_PRESENT:
    printf("0x%02lX\n", response->data_value.u.unsigned8);
    break;
  case CANopen_Value_unsigned16_PRESENT:
    printf("0x%04lX\n", response->data_value.u.unsigned16);
    break;
  case CANopen_Value_unsigned32_PRESENT:
    printf("0x%08lX\n", response->data_value.u.unsigned32);
    break;
  case CANopen_Value_signed8_PRESENT:
    printf("0x%02lX (%li)\n", response->data_value.u.signed8,
           response->data_value.u.signed8);
    break;
  case CANopen_Value_signed16_PRESENT:
    printf("0x%04lX (%li)\n", response->data_value.u.signed16,
           response->data_value.u.signed16);
    if (expectedValue != response->data_value.u.unsigned16) {
      printf(
          "[control] !!! DEMO FAILED !!! Unexpected value fetched from OD!\n");
      demoFinished = true;
    }
    break;
  case CANopen_Value_signed32_PRESENT:
    printf("0x%08lX (%li)\n", response->data_value.u.signed32,
           response->data_value.u.signed32);
    break;
  case CANopen_Value_real32_PRESENT:
    printf("%lf\n", response->data_value.u.real32);
    break;
  default:
    printf("(unknown type)\n");
    break;
  }
}

void control_PI_objdict_set_result(
    const asn1SccCANopen_ObjDict_Operation_Result *result) {
  if (*result != CANopen_ObjDict_Operation_Result_success) {
    printf("[control] !!! DEMO FAILED !!! Setting the value in OD has failed "
           "due to: %s\n",
           objdict_operation_result_to_string(*result));
    demoFinished = true;
  }
}

void control_PI_slave_command_result(
    const asn1SccCANopen_Slave_Command_Result *result) {
  if (result->success) {
    printf("[control] Slave command %s to NodeID 0x%02lX has succeeded!\n",
           nmt_command_to_string(result->command), result->node_id);
  } else {
    printf("[control] Slave command %s to NodeID 0x%02lX has failed (the node "
           "is most likely not on the network - non-critical failure)\n",
           nmt_command_to_string(result->command), result->node_id);
  }
}
