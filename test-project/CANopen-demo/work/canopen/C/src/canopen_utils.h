#pragma once

#include "dataview-uniq.h"

#include <lely/co/nmt.h>
#include <lely/co/type.h>

asn1SccCANopen_NMT_Command map_nmt_command_to_asn(co_unsigned8_t cs);

co_unsigned8_t map_nmt_command_from_asn(asn1SccCANopen_NMT_Command cmd);

asn1SccCANopen_NMT_State map_nmt_state_to_asn(co_unsigned8_t st);

co_unsigned8_t map_nmt_state_from_asn(asn1SccCANopen_NMT_State state);

const char *map_nmt_command_to_string(co_unsigned8_t cs);

const char *map_nmt_state_to_string(co_unsigned8_t st);

asn1SccCANopen_NMT_HeartbeatState
map_nmt_ec_state_to_asn(co_nmt_ec_state_t state);

const char *map_nmt_ec_state_to_string(co_nmt_ec_state_t state);

asn1SccCANopen_NMT_HeartbeatReason
map_nmt_ec_reason_to_asn(co_nmt_ec_reason_t reason);

const char *map_nmt_ec_reason_to_string(co_nmt_ec_reason_t reason);
