#include "canopen_utils.h"
#include "debug.h"

asn1SccCANopen_NMT_Command map_nmt_command_to_asn(co_unsigned8_t cs) {
  switch (cs) {
  case CO_NMT_CS_START:
    return asn1SccCANopen_NMT_Command_start;
  case CO_NMT_CS_STOP:
    return asn1SccCANopen_NMT_Command_stop;
  case CO_NMT_CS_ENTER_PREOP:
    return asn1SccCANopen_NMT_Command_enter_preop;
  case CO_NMT_CS_RESET_NODE:
    return asn1SccCANopen_NMT_Command_reset_node;
  case CO_NMT_CS_RESET_COMM:
    return asn1SccCANopen_NMT_Command_reset_comm;
  default:
    DEBUG_PRINT("[CANopen] Warning: Unknown NMT command: 0x%02x\n", cs);
    return asn1SccCANopen_NMT_Command_start;
  }
}

co_unsigned8_t map_nmt_command_from_asn(asn1SccCANopen_NMT_Command cmd) {
  switch (cmd) {
  case asn1SccCANopen_NMT_Command_start:
    return CO_NMT_CS_START;
  case asn1SccCANopen_NMT_Command_stop:
    return CO_NMT_CS_STOP;
  case asn1SccCANopen_NMT_Command_enter_preop:
    return CO_NMT_CS_ENTER_PREOP;
  case asn1SccCANopen_NMT_Command_reset_node:
    return CO_NMT_CS_RESET_NODE;
  case asn1SccCANopen_NMT_Command_reset_comm:
    return CO_NMT_CS_RESET_COMM;
  default:
    DEBUG_PRINT("[CANopen] Warning: Unknown ASN.1 NMT command: %d\n", cmd);
    return CO_NMT_CS_START;
  }
}

asn1SccCANopen_NMT_State map_nmt_state_to_asn(co_unsigned8_t st) {
  // Mask off the toggle bit
  co_unsigned8_t state = st & ~CO_NMT_ST_TOGGLE;

  switch (state) {
  case CO_NMT_ST_BOOTUP:
    return asn1SccCANopen_NMT_State_bootup;
  case CO_NMT_ST_STOP:
    return asn1SccCANopen_NMT_State_stop;
  case CO_NMT_ST_START:
    return asn1SccCANopen_NMT_State_start;
  case CO_NMT_ST_RESET_NODE:
    return asn1SccCANopen_NMT_State_reset_node;
  case CO_NMT_ST_RESET_COMM:
    return asn1SccCANopen_NMT_State_reset_comm;
  case CO_NMT_ST_PREOP:
    return asn1SccCANopen_NMT_State_preop;
  default:
    DEBUG_PRINT("[CANopen] Warning: Unknown NMT state: 0x%02x\n", st);
    return asn1SccCANopen_NMT_State_bootup;
  }
}

co_unsigned8_t map_nmt_state_from_asn(asn1SccCANopen_NMT_State state) {
  switch (state) {
  case asn1SccCANopen_NMT_State_bootup:
    return CO_NMT_ST_BOOTUP;
  case asn1SccCANopen_NMT_State_stop:
    return CO_NMT_ST_STOP;
  case asn1SccCANopen_NMT_State_start:
    return CO_NMT_ST_START;
  case asn1SccCANopen_NMT_State_reset_node:
    return CO_NMT_ST_RESET_NODE;
  case asn1SccCANopen_NMT_State_reset_comm:
    return CO_NMT_ST_RESET_COMM;
  case asn1SccCANopen_NMT_State_preop:
    return CO_NMT_ST_PREOP;
  default:
    DEBUG_PRINT("[CANopen] Warning: Unknown ASN.1 NMT state: %d\n", state);
    return CO_NMT_ST_BOOTUP;
  }
}

const char *map_nmt_command_to_string(co_unsigned8_t cs) {
  switch (cs) {
  case CO_NMT_CS_START:
    return "START";
  case CO_NMT_CS_STOP:
    return "STOP";
  case CO_NMT_CS_ENTER_PREOP:
    return "ENTER_PREOP";
  case CO_NMT_CS_RESET_NODE:
    return "RESET_NODE";
  case CO_NMT_CS_RESET_COMM:
    return "RESET_COMM";
  default:
    return "UNKNOWN";
  }
}

const char *map_nmt_state_to_string(co_unsigned8_t st) {
  // Mask off the toggle bit
  co_unsigned8_t state = st & ~CO_NMT_ST_TOGGLE;

  switch (state) {
  case CO_NMT_ST_BOOTUP:
    return "BOOTUP";
  case CO_NMT_ST_STOP:
    return "STOP";
  case CO_NMT_ST_START:
    return "START";
  case CO_NMT_ST_RESET_NODE:
    return "RESET_NODE";
  case CO_NMT_ST_RESET_COMM:
    return "RESET_COMM";
  case CO_NMT_ST_PREOP:
    return "PREOP";
  default:
    return "UNKNOWN";
  }
}

asn1SccCANopen_NMT_HeartbeatState
map_nmt_ec_state_to_asn(co_nmt_ec_state_t state) {
  switch (state) {
  case CO_NMT_EC_OCCURRED:
    return asn1SccCANopen_NMT_HeartbeatState_occurred;
  case CO_NMT_EC_RESOLVED:
    return asn1SccCANopen_NMT_HeartbeatState_resolved;
  default:
    DEBUG_PRINT("[CANopen] Warning: Unknown NMT EC state: %d\n", state);
    return asn1SccCANopen_NMT_HeartbeatState_occurred;
  }
}

const char *map_nmt_ec_state_to_string(co_nmt_ec_state_t state) {
  switch (state) {
  case CO_NMT_EC_OCCURRED:
    return "OCCURRED";
  case CO_NMT_EC_RESOLVED:
    return "RESOLVED";
  default:
    return "UNKNOWN";
  }
}

asn1SccCANopen_NMT_HeartbeatReason
map_nmt_ec_reason_to_asn(co_nmt_ec_reason_t reason) {
  switch (reason) {
  case CO_NMT_EC_TIMEOUT:
    return asn1SccCANopen_NMT_HeartbeatReason_timeout;
  case CO_NMT_EC_STATE:
    return asn1SccCANopen_NMT_HeartbeatReason_state_change;
  default:
    DEBUG_PRINT("[CANopen] Warning: Unknown NMT EC reason: %d\n", reason);
    return asn1SccCANopen_NMT_HeartbeatReason_timeout;
  }
}

const char *map_nmt_ec_reason_to_string(co_nmt_ec_reason_t reason) {
  switch (reason) {
  case CO_NMT_EC_TIMEOUT:
    return "TIMEOUT";
  case CO_NMT_EC_STATE:
    return "STATE_CHANGE";
  default:
    return "UNKNOWN";
  }
}
