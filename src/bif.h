#ifndef BIF_PARSER_H
#define BIF_PARSER_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <linux/limits.h>

#define BIF_SUCCESS                0
#define BIF_ERROR_NOFILE           1
#define BIF_ERROR_PARSER           2
#define BIF_ERROR_UNSUPPORTED_ATTR 3
#define BIF_ERROR_UNINITIALIZED    4
#define BIF_ERROR_UNSUPPORTED_VAL  5

#define BIF_ARCH_ZYNQ              ((uint8_t) 1 << 0)
#define BIF_ARCH_ZYNQMP            ((uint8_t) 1 << 1)

#define BIF_CFG_NO_SORT_BY_OFFSET  ((uint8_t) 1 << 0)
#define BIF_CFG_VERBOSE            ((uint8_t) 1 << 7)

typedef enum partition_owner_e {
  OWNER_FSBL,
  OWNER_UBOOT
} partition_owner_t;

typedef enum destination_device_e {
  DST_DEV_UNDEF = -1,
  DST_DEV_PS,
  DST_DEV_PL
} destination_device_t;

typedef enum destination_cpu_e {
  DST_CPU_UNDEF = -1,
  DST_CPU_A53_0,
  DST_CPU_A53_1,
  DST_CPU_A53_2,
  DST_CPU_A53_3,
  DST_CPU_R5_0,
  DST_CPU_R5_1,
  DST_CPU_R5_LOCKSTEP
} destination_cpu_t;

typedef enum exception_level_e {
  EL_UNDEF = -1,
  EL_0,
  EL_1,
  EL_2,
  EL_3
} exception_level_t;

typedef struct bif_node_t {
  char fname[PATH_MAX];

  /* supported common attributes */
  uint8_t bootloader; /* boolean */
  uint32_t load;
  uint32_t offset;
  partition_owner_t partition_owner;

  /* supported zynqmp attributes */
  uint8_t fsbl_config; /* boolean */
  uint8_t pmufw_image; /* boolean */
  destination_device_t destination_device;
  destination_cpu_t destination_cpu;
  exception_level_t exception_level;

  /* special, non-bootgen features */
  uint8_t is_file; /* for now equal to !fsbl_config */
  uint8_t numbits;
} bif_node_t;

typedef struct bif_cfg_t {
  uint8_t arch;
  uint8_t flags;

  uint16_t nodes_num;
  uint16_t nodes_avail;

  bif_node_t *nodes;
  const char *output_bitstream_filename;
} bif_cfg_t;

int init_bif_cfg(bif_cfg_t *cfg);
int deinit_bif_cfg(bif_cfg_t *cfg);
int parse_bif(const char* fname, bif_cfg_t *cfg);

#endif /* BIF_PARSER_H */
