#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <lely/can/msg.h>
#include <lely/can/net.h>
#include <lely/co/co.h>
#include <lely/co/dev.h>
#include <lely/co/nmt.h>
#include <lely/co/obj.h>
#include <lely/co/sync.h>
#include <lely/co/type.h>
#include <lely/util/cmp.h>
#include <lely/util/memory.h>
#include <lely/util/mempool.h>
#include <lely/util/time.h>

#include "canopen.h"
#include "canopen_utils.h"
#include "config.h"
#include "dataview-uniq.h"
#include "master_dev.h"

#ifdef CANOPEN_DEBUG
#include <stdio.h>
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) ((void)0)
#endif

#define MEMORY_POOL_SIZE (10 * 1024 * 1024)

static char memory[MEMORY_POOL_SIZE] __attribute__((aligned)) = {0};
static struct mempool memory_pool;
static can_net_t *net = NULL;
static co_dev_t *dev = NULL;
static co_nmt_t *nmt = NULL;
static co_sync_t *sync = NULL;
static struct timespec startupTime;

static alloc_t *allocator_init(void) {
  alloc_t *const alloc = mempool_init(&memory_pool, memory, sizeof(memory));
  assert(alloc);
  return alloc;
}

static struct timespec get_current_time() {
  asn1SccTime currentTime;
  canopen_RI_get_elapsed_time(&currentTime);
  return convert_asn_time_to_timespec(currentTime);
}

static struct timespec get_time_from_startup() {
  struct timespec currentTime = get_current_time();
  struct timespec elapsed;

  elapsed.tv_sec = currentTime.tv_sec - startupTime.tv_sec;
  elapsed.tv_nsec = currentTime.tv_nsec - startupTime.tv_nsec;

  // Handle negative nanoseconds
  if (elapsed.tv_nsec < 0) {
    elapsed.tv_sec -= 1;
    elapsed.tv_nsec += 1000000000; // 1 second in nanoseconds
  }

  return elapsed;
}

static int send_callback(const struct can_msg *const msg, uint_least8_t busId,
                         void *userdata) {
  (void)busId;
  (void)userdata;

  if (msg == NULL) {
    return -1;
  }

  // Convert can_msg to asn1SccCan_Frame
  asn1SccCan_Frame frame;
  asn1SccCan_Frame_Initialize(&frame);

  // Convert CAN ID
  if (msg->flags & CAN_FLAG_IDE) {
    // Extended ID (29-bit)
    frame.id.kind = Can_Id_extended_id_PRESENT;
    frame.id.u.extended_id = msg->id & CAN_MASK_EID;
  } else {
    // Standard ID (11-bit)
    frame.id.kind = Can_Id_standard_id_PRESENT;
    frame.id.u.standard_id = msg->id & CAN_MASK_BID;
  }

  // Convert data
  frame.data.nCount = (msg->len > 8) ? 8 : msg->len;
  for (int i = 0; i < frame.data.nCount; i++) {
    frame.data.arr[i] = msg->data[i];
  }

  // Transmit the frame
  canopen_RI_transmit_data(&frame);

  return 0;
}

void canopen_PI_receive_data(const asn1SccCan_Frame *frame_data) {
  if (net == NULL || frame_data == NULL) {
    return;
  }

  struct can_msg frame;
  memset(&frame, 0, sizeof(frame));

  // Convert asn1SccCan_Frame to can_msg
  // Convert CAN ID
  if (frame_data->id.kind == Can_Id_extended_id_PRESENT) {
    // Extended ID (29-bit)
    frame.flags |= CAN_FLAG_IDE;
    frame.id = frame_data->id.u.extended_id & CAN_MASK_EID;
  } else if (frame_data->id.kind == Can_Id_standard_id_PRESENT) {
    // Standard ID (11-bit)
    frame.id = frame_data->id.u.standard_id & CAN_MASK_BID;
  }

  // Convert data
  frame.len = (frame_data->data.nCount > 8) ? 8 : frame_data->data.nCount;
  for (int i = 0; i < frame.len; i++) {
    frame.data[i] = frame_data->data.arr[i];
  }

  can_net_recv(net, &frame, 0);
}

static void nmt_command_indicator(co_nmt_t *nmt_, co_unsigned8_t cs,
                                  void *data) {
  assert(nmt_ == nmt);
  (void)data;

  DEBUG_PRINT(
      "[CANopen] NMT command indicator triggered; received command: %X (%s) \n",
      cs, map_nmt_command_to_string(cs));

  const asn1SccCANopen_NMT_Command asnCs = map_nmt_command_to_asn(cs);
  canopen_RI_nmt_command_indicator(&asnCs);
}

static void nmt_state_indicator(co_nmt_t *nmt_, co_unsigned8_t id,
                                co_unsigned8_t st, void *data) {
  assert(nmt_ == nmt);
  (void)data;
  DEBUG_PRINT(
      "[CANopen] NMT state indicator triggered; ID: %X, state: %X (%s)\n", id,
      st, map_nmt_state_to_string(st));

  const asn1SccCANopen_NMT_State asnSt = map_nmt_state_to_asn(st);
  const asn1SccCANopen_Unsigned8 asnId = id;
  canopen_RI_nmt_state_indicator(&asnId, &asnSt);
}

static void nmt_hearbeat_indicator(co_nmt_t *nmt_, co_unsigned8_t id,
                                   co_nmt_ec_state_t state_,
                                   co_nmt_ec_reason_t reason, void *data) {
  assert(nmt_ == nmt);
  (void)data;
  DEBUG_PRINT(
      "[CANopen] NMT heartbeat received from node %02X, state: %X (%s), "
      "reason: %X (%s)\n",
      id, state_, map_nmt_ec_state_to_string(state_), reason,
      map_nmt_ec_reason_to_string(reason));

  const asn1SccCANopen_NodeID asnNodeId = id;
  const asn1SccCANopen_NMT_HeartbeatState asnHbState =
      map_nmt_ec_state_to_asn(state_);
  const asn1SccCANopen_NMT_HeartbeatReason asnHbReason =
      map_nmt_ec_reason_to_asn(reason);
  canopen_RI_nmt_heartbeat_indicator(&asnNodeId, &asnHbState, &asnHbReason);
}

static void nmt_sync_indicator(co_sync_t *sync_, co_unsigned8_t cnt,
                               void *data) {
  assert(sync_ == sync);
  (void)data;
  DEBUG_PRINT("[CANopen] NMT sync received, counter: %X\n", cnt);

  const asn1SccCANopen_Unsigned8 asnCounter = cnt;
  canopen_RI_nmt_sync_indicator(&asnCounter);
}

void canopen_startup(void) {
  // Startup left empty, initialization implemented in canopen_PI_init
}

void canopen_PI_init(void) {
  if (net != NULL) {
    // Already initialized
    DEBUG_PRINT("[CANopen] Already initialized, ignoring init...\n");
    return;
  }

  DEBUG_PRINT("[CANopen] Initializing...\n");

  // Initialize device from static object dictionary
  dev = master_dev_init();
  if (dev == NULL) {
    DEBUG_PRINT("[CANopen] ERROR: Failed to initialize device\n");
    return;
  }

  // Create CAN network
  net = can_net_create(allocator_init(), 0);
  if (net == NULL) {
    DEBUG_PRINT("[CANopen] ERROR: Failed to create CAN network\n");
    return;
  }

  // Create NMT service
  nmt = co_nmt_create(net, dev);
  if (nmt == NULL) {
    DEBUG_PRINT("[CANopen] ERROR: Failed to create NMT service\n");
    return;
  }

  sync = co_nmt_get_sync(nmt);
  if (sync == NULL) {
    DEBUG_PRINT(
        "[CANopen] ERROR: NMT service manager failed to create SYNC service\n");
    return;
  }

  // Set network callbacks
  can_net_set_send_func(net, send_callback, NULL);
  co_nmt_set_cs_ind(nmt, nmt_command_indicator, NULL);
  co_nmt_set_st_ind(nmt, nmt_state_indicator, NULL);
  co_nmt_set_hb_ind(nmt, nmt_hearbeat_indicator, NULL);
  co_sync_set_ind(sync, nmt_sync_indicator, NULL);

  startupTime = get_current_time();

  DEBUG_PRINT("[CANopen] Initialization complete\n");
}

void canopen_PI_canopen_network_tick(void) {
  if (net == NULL) {
    return;
  }

  struct timespec currentTime = get_time_from_startup();
  can_net_set_time(net, &currentTime);

  // TODO: this is debug log, probably kill
  static uint32_t counter = 0;
  if (counter % 100 == 0)
    DEBUG_PRINT("[CANopen] Current network time: %lds, %ldns\n",
                currentTime.tv_sec, currentTime.tv_nsec);

  counter++;
}

void canopen_PI_change_node_state(const asn1SccCANopen_NMT_State *state) {
  if (net == NULL || nmt == NULL || state == NULL) {
    return;
  }

  // Map ASN.1 NMT state to Lely CANopen NMT command specifier
  co_unsigned8_t cs;
  switch (*state) {
  case asn1SccCANopen_NMT_State_start:
    cs = CO_NMT_CS_START;
    break;
  case asn1SccCANopen_NMT_State_stop:
    cs = CO_NMT_CS_STOP;
    break;
  case asn1SccCANopen_NMT_State_preop:
    cs = CO_NMT_CS_ENTER_PREOP;
    break;
  case asn1SccCANopen_NMT_State_reset_node:
    cs = CO_NMT_CS_RESET_NODE;
    break;
  case asn1SccCANopen_NMT_State_reset_comm:
    cs = CO_NMT_CS_RESET_COMM;
    break;
  case asn1SccCANopen_NMT_State_bootup:
    // Bootup is not a command that can be issued externally
    DEBUG_PRINT("[CANopen] Warning: Cannot transition to bootup state via "
                "command\n");
    return;
  default:
    DEBUG_PRINT("[CANopen] Error: Unknown NMT state: %d\n", *state);
    return;
  }

  // Issue the NMT command
  DEBUG_PRINT("[CANopen] Changing network state to: %d (command: 0x%02X)\n",
              *state, cs);
  int result = co_nmt_cs_ind(nmt, cs);
  if (result != 0) {
    DEBUG_PRINT(
        "[CANopen] Error: Failed to change network state (error code: %d)\n",
        result);
  }
}

void canopen_PI_reset_node(void) {
  if (nmt == NULL) {
    return;
  }

  // Issue NMT reset node command
  DEBUG_PRINT("[CANopen] Network reset triggered!\n");
  co_nmt_cs_ind(nmt, CO_NMT_CS_RESET_NODE);
}

void canopen_PI_issue_slave_command(
    const asn1SccCANopen_NodeID *asnNodeId,
    const asn1SccCANopen_NMT_Command *asnCommand) {

  co_unsigned8_t nodeId = *asnNodeId;
  co_unsigned8_t command = map_nmt_command_from_asn(*asnCommand);
  DEBUG_PRINT("[CANopen] Issuing command %X (%s) to node %X\n", command,
              map_nmt_command_to_string(command), nodeId);

  if (co_nmt_cs_req(nmt, command, nodeId) != 0) {
    DEBUG_PRINT("[CANopen] NMT command request %X (%s) to node %X failed!\n",
                command, map_nmt_command_to_string(command), nodeId);
  }
}

void canopen_PI_set_object_dictionary_data(
    const asn1SccCANopen_Object_Index *index,
    const asn1SccCANopen_Subobject_Index *subindex,
    const asn1SccCANopen_Value *value) {
  if (dev == NULL || index == NULL || subindex == NULL || value == NULL) {
    return;
  }

  co_obj_t *obj = co_dev_find_obj(dev, (co_unsigned16_t)(*index));
  if (obj == NULL) {
    DEBUG_PRINT("[CANopen] ERROR: Object %04lX not found\n", *index);
    return;
  }

  co_sub_t *sub = co_obj_find_sub(obj, (co_unsigned8_t)(*subindex));
  if (sub == NULL) {
    DEBUG_PRINT("[CANopen] ERROR: Sub-object %04lX:%02lX not found\n", *index,
                *subindex);
    return;
  }

  // Write value based on the choice type
  size_t bytes_written = 0;
  switch (value->kind) {
  case CANopen_Value_boolean_d_PRESENT:
    bytes_written = co_sub_set_val_b(sub, value->u.boolean_d);
    break;
  case CANopen_Value_unsigned8_PRESENT:
    bytes_written = co_sub_set_val_u8(sub, (co_unsigned8_t)value->u.unsigned8);
    break;
  case CANopen_Value_unsigned16_PRESENT:
    bytes_written =
        co_sub_set_val_u16(sub, (co_unsigned16_t)value->u.unsigned16);
    break;
  case CANopen_Value_unsigned24_PRESENT:
    bytes_written =
        co_sub_set_val_u24(sub, (co_unsigned24_t)value->u.unsigned24);
    break;
  case CANopen_Value_unsigned32_PRESENT:
    bytes_written =
        co_sub_set_val_u32(sub, (co_unsigned32_t)value->u.unsigned32);
    break;
  case CANopen_Value_signed8_PRESENT:
    bytes_written = co_sub_set_val_i8(sub, (co_integer8_t)value->u.signed8);
    break;
  case CANopen_Value_signed16_PRESENT:
    bytes_written = co_sub_set_val_i16(sub, (co_integer16_t)value->u.signed16);
    break;
  case CANopen_Value_signed24_PRESENT:
    bytes_written = co_sub_set_val_i24(sub, (co_integer24_t)value->u.signed24);
    break;
  case CANopen_Value_signed32_PRESENT:
    bytes_written = co_sub_set_val_i32(sub, (co_integer32_t)value->u.signed32);
    break;
  case CANopen_Value_real32_PRESENT:
    bytes_written = co_sub_set_val_r32(sub, (co_real32_t)value->u.real32);
    break;
  default:
    DEBUG_PRINT("[CANopen] ERROR: Unsupported value type 0x%04X\n",
                value->kind);
    return;
  }

  if (bytes_written == 0) {
    DEBUG_PRINT("[CANopen] ERROR: Failed to write value to OD %04lX:%02lX\n",
                *index, *subindex);
    return;
  }

  DEBUG_PRINT("[CANopen] Set OD %04lX:%02lX successful (%zu bytes written)\n",
              *index, *subindex, bytes_written);
}

void canopen_PI_get_object_dictionary_data(
    const asn1SccCANopen_Object_Index *index,
    const asn1SccCANopen_Subobject_Index *subindex,
    asn1SccCANopen_Value *value) {
  if (dev == NULL || index == NULL || subindex == NULL || value == NULL) {
    return;
  }

  co_obj_t *obj = co_dev_find_obj(dev, (co_unsigned16_t)(*index));
  if (obj == NULL) {
    DEBUG_PRINT("[CANopen] ERROR: Object %04lX not found\n", *index);
    return;
  }

  co_sub_t *sub = co_obj_find_sub(obj, (co_unsigned8_t)(*subindex));
  if (sub == NULL) {
    DEBUG_PRINT("[CANopen] ERROR: Sub-object %04lX:%02lX not found\n", *index,
                *subindex);
    return;
  }

  // Read value based on the object dictionary data type
  co_unsigned16_t type = co_sub_get_type(sub);

  switch (type) {
  case CO_DEFTYPE_BOOLEAN:
    value->kind = CANopen_Value_boolean_d_PRESENT;
    value->u.boolean_d = (co_sub_get_val_b(sub));
    break;
  case CO_DEFTYPE_UNSIGNED8:
    value->kind = CANopen_Value_unsigned8_PRESENT;
    value->u.unsigned8 = co_sub_get_val_u8(sub);
    break;
  case CO_DEFTYPE_UNSIGNED16:
    value->kind = CANopen_Value_unsigned16_PRESENT;
    value->u.unsigned16 = co_sub_get_val_u16(sub);
    break;
  case CO_DEFTYPE_UNSIGNED24:
    value->kind = CANopen_Value_unsigned24_PRESENT;
    value->u.unsigned24 = co_sub_get_val_u24(sub);
    break;
  case CO_DEFTYPE_UNSIGNED32:
    value->kind = CANopen_Value_unsigned32_PRESENT;
    value->u.unsigned32 = co_sub_get_val_u32(sub);
    break;
  case CO_DEFTYPE_INTEGER8:
    value->kind = CANopen_Value_signed8_PRESENT;
    value->u.signed16 = co_sub_get_val_i8(sub);
    break;
  case CO_DEFTYPE_INTEGER16:
    value->kind = CANopen_Value_signed16_PRESENT;
    value->u.signed16 = co_sub_get_val_i16(sub);
    break;
  case CO_DEFTYPE_INTEGER24:
    value->kind = CANopen_Value_signed24_PRESENT;
    value->u.signed24 = co_sub_get_val_i24(sub);
    break;
  case CO_DEFTYPE_INTEGER32:
    value->kind = CANopen_Value_signed32_PRESENT;
    value->u.signed32 = co_sub_get_val_i32(sub);
    break;
  case CO_DEFTYPE_REAL32:
    value->kind = CANopen_Value_real32_PRESENT;
    value->u.real32 = co_sub_get_val_r32(sub);
    break;
  default:
    DEBUG_PRINT("[CANopen] ERROR: Unsupported value type 0x%04X\n", type);
    break;
  }

  DEBUG_PRINT("[CANopen] Get OD %04lX:%02lX successful\n", *index, *subindex);
}
