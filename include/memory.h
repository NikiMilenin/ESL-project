#ifndef MEMORY_H
#define MEMORY_H
#include "nrf_bootloader_info.h"
#include "nrf_dfu_types.h"
#include "nrfx_nvmc.h"
#include "leds.h"
#include <stdint.h>

void write_state(uint32_t* arr);
void read_state(uint32_t* arr);
uint32_t* get_read_ptr();
uint32_t * get_write_ptr(uint32_t* value);
void clear_mem();

#endif