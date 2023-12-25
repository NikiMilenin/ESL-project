#ifndef MEMORY_H
#define MEMORY_H
#include "nrf_bootloader_info.h"
#include "nrf_dfu_types.h"
#include "nrfx_nvmc.h"
#include "leds.h"
#include <stdint.h>

void write_arr(uint32_t*, int);
void read_arr(uint32_t*, int);
void clear_mem();

#endif