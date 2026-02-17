#if !LELY_NO_MALLOC
#error Static object dictionaries are only supported when dynamic memory allocation is disabled.
#endif

#include <lely/co/detail/dev.h>
#include <lely/co/detail/obj.h>
#include <lely/util/cmp.h>

#define CO_DEV_STRING(s) s

static co_dev_t master_dev = {
	.netid = 0,
	.id = 2,
	.tree = { .cmp = &uint16_cmp },
#if !LELY_NO_CO_OBJ_NAME
	.name = CO_DEV_STRING(""),
	.vendor_name = CO_DEV_STRING("N7 Space Sp. z o.o."),
#endif
	.vendor_id = 0x004E3753,
#if !LELY_NO_CO_OBJ_NAME
	.product_name = CO_DEV_STRING(""),
#endif
	.product_code = 0x00000000,
	.revision = 0x00000000,
#if !LELY_NO_CO_OBJ_NAME
	.order_code = CO_DEV_STRING(""),
#endif
	.baud = 0 | CO_BAUD_10 | CO_BAUD_20 | CO_BAUD_50 | CO_BAUD_125 | CO_BAUD_250 | CO_BAUD_500 | CO_BAUD_800 | CO_BAUD_1000,
	.rate = 1000,
	.lss = 0,
	.dummy = 0x00000000
};

static struct {
	co_unsigned32_t sub0;
} master_dev_1000_val = {
	.sub0 = 0x00000000,
};

static co_obj_t master_dev_1000 = {
	.node = { .key = &master_dev_1000.idx },
	.idx = 0x1000,
	.code = CO_OBJECT_VAR,
#if !LELY_NO_CO_OBJ_NAME
	.name = CO_DEV_STRING("Device type"),
#endif
	.tree = { .cmp = &uint8_cmp },
	.val = &master_dev_1000_val,
	.size = sizeof(master_dev_1000_val)
};

static co_sub_t master_dev_1000sub0 = {
	.node = { .key = &master_dev_1000sub0.subidx },
	.subidx = 0x00,
	.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_NAME
	.name = CO_DEV_STRING("Device type"),
#endif
#if !LELY_NO_CO_OBJ_LIMITS
	.min = { .u32 = CO_UNSIGNED32_MIN },
	.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
	.def = { .u32 = 0x00000000 },
#endif
	.val = &master_dev_1000_val.sub0,
	.access = CO_ACCESS_RO,
	.pdo_mapping = 0,
	.flags = 0,
	.dn_ind = &co_sub_default_dn_ind,
#if !LELY_NO_CO_OBJ_UPLOAD
	.up_ind = &co_sub_default_up_ind
#endif
};

static struct {
	co_unsigned8_t sub0;
} master_dev_1001_val = {
	.sub0 = 0x00,
};

static co_obj_t master_dev_1001 = {
	.node = { .key = &master_dev_1001.idx },
	.idx = 0x1001,
	.code = CO_OBJECT_VAR,
#if !LELY_NO_CO_OBJ_NAME
	.name = CO_DEV_STRING("Error register"),
#endif
	.tree = { .cmp = &uint8_cmp },
	.val = &master_dev_1001_val,
	.size = sizeof(master_dev_1001_val)
};

static co_sub_t master_dev_1001sub0 = {
	.node = { .key = &master_dev_1001sub0.subidx },
	.subidx = 0x00,
	.type = CO_DEFTYPE_UNSIGNED8,
#if !LELY_NO_CO_OBJ_NAME
	.name = CO_DEV_STRING("Error register"),
#endif
#if !LELY_NO_CO_OBJ_LIMITS
	.min = { .u8 = CO_UNSIGNED8_MIN },
	.max = { .u8 = CO_UNSIGNED8_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
	.def = { .u8 = 0x00 },
#endif
	.val = &master_dev_1001_val.sub0,
	.access = CO_ACCESS_RO,
	.pdo_mapping = 0,
	.flags = 0,
	.dn_ind = &co_sub_default_dn_ind,
#if !LELY_NO_CO_OBJ_UPLOAD
	.up_ind = &co_sub_default_up_ind
#endif
};

static struct {
	co_unsigned32_t sub0;
} master_dev_1005_val = {
	.sub0 = 0x40000080,
};

static co_obj_t master_dev_1005 = {
	.node = { .key = &master_dev_1005.idx },
	.idx = 0x1005,
	.code = CO_OBJECT_VAR,
#if !LELY_NO_CO_OBJ_NAME
	.name = CO_DEV_STRING("COB-ID SYNC"),
#endif
	.tree = { .cmp = &uint8_cmp },
	.val = &master_dev_1005_val,
	.size = sizeof(master_dev_1005_val)
};

static co_sub_t master_dev_1005sub0 = {
	.node = { .key = &master_dev_1005sub0.subidx },
	.subidx = 0x00,
	.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_NAME
	.name = CO_DEV_STRING("COB-ID SYNC"),
#endif
#if !LELY_NO_CO_OBJ_LIMITS
	.min = { .u32 = CO_UNSIGNED32_MIN },
	.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
	.def = { .u32 = 0x00000080 },
#endif
	.val = &master_dev_1005_val.sub0,
	.access = CO_ACCESS_RW,
	.pdo_mapping = 0,
	.flags = 0 | CO_OBJ_FLAGS_PARAMETER_VALUE,
	.dn_ind = &co_sub_default_dn_ind,
#if !LELY_NO_CO_OBJ_UPLOAD
	.up_ind = &co_sub_default_up_ind
#endif
};

static struct {
	co_unsigned32_t sub0;
} master_dev_1006_val = {
	.sub0 = 0x0007A120,
};

static co_obj_t master_dev_1006 = {
	.node = { .key = &master_dev_1006.idx },
	.idx = 0x1006,
	.code = CO_OBJECT_VAR,
#if !LELY_NO_CO_OBJ_NAME
	.name = CO_DEV_STRING("Communication cycle period"),
#endif
	.tree = { .cmp = &uint8_cmp },
	.val = &master_dev_1006_val,
	.size = sizeof(master_dev_1006_val)
};

static co_sub_t master_dev_1006sub0 = {
	.node = { .key = &master_dev_1006sub0.subidx },
	.subidx = 0x00,
	.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_NAME
	.name = CO_DEV_STRING("Communication cycle period"),
#endif
#if !LELY_NO_CO_OBJ_LIMITS
	.min = { .u32 = CO_UNSIGNED32_MIN },
	.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
	.def = { .u32 = 0x0007A120 },
#endif
	.val = &master_dev_1006_val.sub0,
	.access = CO_ACCESS_RW,
	.pdo_mapping = 0,
	.flags = 0,
	.dn_ind = &co_sub_default_dn_ind,
#if !LELY_NO_CO_OBJ_UPLOAD
	.up_ind = &co_sub_default_up_ind
#endif
};

static struct {
	co_unsigned8_t sub0;
	co_unsigned32_t sub1;
	co_unsigned32_t sub2;
	co_unsigned32_t sub3;
	co_unsigned32_t sub4;
} master_dev_1018_val = {
	.sub0 = 0x04,
	.sub1 = 0x004E3753,
	.sub2 = 0x00000000,
	.sub3 = 0x00000000,
	.sub4 = 0x00000000,
};

static co_obj_t master_dev_1018 = {
	.node = { .key = &master_dev_1018.idx },
	.idx = 0x1018,
	.code = CO_OBJECT_RECORD,
#if !LELY_NO_CO_OBJ_NAME
	.name = CO_DEV_STRING("Identity object"),
#endif
	.tree = { .cmp = &uint8_cmp },
	.val = &master_dev_1018_val,
	.size = sizeof(master_dev_1018_val)
};

static co_sub_t master_dev_1018sub0 = {
	.node = { .key = &master_dev_1018sub0.subidx },
	.subidx = 0x00,
	.type = CO_DEFTYPE_UNSIGNED8,
#if !LELY_NO_CO_OBJ_NAME
	.name = CO_DEV_STRING("Highest sub-index supported"),
#endif
#if !LELY_NO_CO_OBJ_LIMITS
	.min = { .u8 = CO_UNSIGNED8_MIN },
	.max = { .u8 = CO_UNSIGNED8_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
	.def = { .u8 = 0x04 },
#endif
	.val = &master_dev_1018_val.sub0,
	.access = CO_ACCESS_CONST,
	.pdo_mapping = 0,
	.flags = 0,
	.dn_ind = &co_sub_default_dn_ind,
#if !LELY_NO_CO_OBJ_UPLOAD
	.up_ind = &co_sub_default_up_ind
#endif
};

static co_sub_t master_dev_1018sub1 = {
	.node = { .key = &master_dev_1018sub1.subidx },
	.subidx = 0x01,
	.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_NAME
	.name = CO_DEV_STRING("Vendor-ID"),
#endif
#if !LELY_NO_CO_OBJ_LIMITS
	.min = { .u32 = CO_UNSIGNED32_MIN },
	.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
	.def = { .u32 = 0x004E3753 },
#endif
	.val = &master_dev_1018_val.sub1,
	.access = CO_ACCESS_RO,
	.pdo_mapping = 0,
	.flags = 0,
	.dn_ind = &co_sub_default_dn_ind,
#if !LELY_NO_CO_OBJ_UPLOAD
	.up_ind = &co_sub_default_up_ind
#endif
};

static co_sub_t master_dev_1018sub2 = {
	.node = { .key = &master_dev_1018sub2.subidx },
	.subidx = 0x02,
	.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_NAME
	.name = CO_DEV_STRING("Product code"),
#endif
#if !LELY_NO_CO_OBJ_LIMITS
	.min = { .u32 = CO_UNSIGNED32_MIN },
	.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
	.def = { .u32 = 0x00000000 },
#endif
	.val = &master_dev_1018_val.sub2,
	.access = CO_ACCESS_RO,
	.pdo_mapping = 0,
	.flags = 0,
	.dn_ind = &co_sub_default_dn_ind,
#if !LELY_NO_CO_OBJ_UPLOAD
	.up_ind = &co_sub_default_up_ind
#endif
};

static co_sub_t master_dev_1018sub3 = {
	.node = { .key = &master_dev_1018sub3.subidx },
	.subidx = 0x03,
	.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_NAME
	.name = CO_DEV_STRING("Revision number"),
#endif
#if !LELY_NO_CO_OBJ_LIMITS
	.min = { .u32 = CO_UNSIGNED32_MIN },
	.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
	.def = { .u32 = 0x00000000 },
#endif
	.val = &master_dev_1018_val.sub3,
	.access = CO_ACCESS_RO,
	.pdo_mapping = 0,
	.flags = 0,
	.dn_ind = &co_sub_default_dn_ind,
#if !LELY_NO_CO_OBJ_UPLOAD
	.up_ind = &co_sub_default_up_ind
#endif
};

static co_sub_t master_dev_1018sub4 = {
	.node = { .key = &master_dev_1018sub4.subidx },
	.subidx = 0x04,
	.type = CO_DEFTYPE_UNSIGNED32,
#if !LELY_NO_CO_OBJ_NAME
	.name = CO_DEV_STRING("Serial number"),
#endif
#if !LELY_NO_CO_OBJ_LIMITS
	.min = { .u32 = CO_UNSIGNED32_MIN },
	.max = { .u32 = CO_UNSIGNED32_MAX },
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
	.def = { .u32 = 0x00000000 },
#endif
	.val = &master_dev_1018_val.sub4,
	.access = CO_ACCESS_RO,
	.pdo_mapping = 0,
	.flags = 0,
	.dn_ind = &co_sub_default_dn_ind,
#if !LELY_NO_CO_OBJ_UPLOAD
	.up_ind = &co_sub_default_up_ind
#endif
};

// suppress missing-prototype warning
co_dev_t * master_dev_init(void);
co_dev_t *
master_dev_init(void) {
	static co_dev_t *dev = NULL;
	if (!dev) {
		dev = &master_dev;

		co_dev_insert_obj(&master_dev, &master_dev_1000);
		co_obj_insert_sub(&master_dev_1000, &master_dev_1000sub0);

		co_dev_insert_obj(&master_dev, &master_dev_1001);
		co_obj_insert_sub(&master_dev_1001, &master_dev_1001sub0);

		co_dev_insert_obj(&master_dev, &master_dev_1005);
		co_obj_insert_sub(&master_dev_1005, &master_dev_1005sub0);

		co_dev_insert_obj(&master_dev, &master_dev_1006);
		co_obj_insert_sub(&master_dev_1006, &master_dev_1006sub0);

		co_dev_insert_obj(&master_dev, &master_dev_1018);
		co_obj_insert_sub(&master_dev_1018, &master_dev_1018sub0);
		co_obj_insert_sub(&master_dev_1018, &master_dev_1018sub1);
		co_obj_insert_sub(&master_dev_1018, &master_dev_1018sub2);
		co_obj_insert_sub(&master_dev_1018, &master_dev_1018sub3);
		co_obj_insert_sub(&master_dev_1018, &master_dev_1018sub4);
	}
	return dev;
}
