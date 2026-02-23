#include "tester.h"
#include <stdio.h>

static const char *nmt_command_to_string(asn1SccCANopen_NMT_Command cmd) {
  switch (cmd) {
  case asn1SccCANopen_NMT_Command_start:
    return "START";
  case asn1SccCANopen_NMT_Command_stop:
    return "STOP";
  case asn1SccCANopen_NMT_Command_enter_preop:
    return "ENTER_PREOP";
  case asn1SccCANopen_NMT_Command_reset_node:
    return "RESET_NODE";
  case asn1SccCANopen_NMT_Command_reset_comm:
    return "RESET_COMM";
  default:
    return "UNKNOWN";
  }
}

static const char *nmt_state_to_string(asn1SccCANopen_NMT_State state) {
  switch (state) {
  case asn1SccCANopen_NMT_State_bootup:
    return "BOOTUP";
  case asn1SccCANopen_NMT_State_stop:
    return "STOP";
  case asn1SccCANopen_NMT_State_start:
    return "START";
  case asn1SccCANopen_NMT_State_reset_node:
    return "RESET_NODE";
  case asn1SccCANopen_NMT_State_reset_comm:
    return "RESET_COMM";
  case asn1SccCANopen_NMT_State_preop:
    return "PREOP";
  default:
    return "UNKNOWN";
  }
}

static const char *hb_state_to_string(asn1SccCANopen_NMT_HeartbeatState state) {
  switch (state) {
  case asn1SccCANopen_NMT_HeartbeatState_occurred:
    return "OCCURRED";
  case asn1SccCANopen_NMT_HeartbeatState_resolved:
    return "RESOLVED";
  default:
    return "UNKNOWN";
  }
}

static const char *
hb_reason_to_string(asn1SccCANopen_NMT_HeartbeatReason reason) {
  switch (reason) {
  case asn1SccCANopen_NMT_HeartbeatReason_timeout:
    return "TIMEOUT";
  case asn1SccCANopen_NMT_HeartbeatReason_state_change:
    return "STATE_CHANGE";
  default:
    return "UNKNOWN";
  }
}

void tester_startup(void) {}

void tester_PI_nmt_command_indicator(
    const asn1SccCANopen_NMT_Command *IN_command) {
  printf("[tester] Received NMT command trigger: 0x%02X (%s)\n", *IN_command,
         nmt_command_to_string(*IN_command));
}

void tester_PI_nmt_state_indicator(
    const asn1SccCANopen_Unsigned8 *IN_nodeid,
    const asn1SccCANopen_NMT_State *IN_nodestate) {
  printf("[tester] Received NMT state change trigger for node 0x%02lX: "
         "0x%02X (%s)\n",
         *IN_nodeid, *IN_nodestate, nmt_state_to_string(*IN_nodestate));
}

void tester_PI_nmt_heartbeat_indicator(
    const asn1SccCANopen_NodeID *nodeId,
    const asn1SccCANopen_NMT_HeartbeatState *hbState,
    const asn1SccCANopen_NMT_HeartbeatReason *hbReason) {
  printf("[tester] Received heartbeat from node %lX, state: %s, reason: %s\n",
         *nodeId, hb_state_to_string(*hbState), hb_reason_to_string(*hbReason));
}

void tester_PI_nmt_sync_indicator(const asn1SccCANopen_Unsigned8 *counter) {
  printf("[tester] SYNC received with counter %lX\n", *counter);
}
