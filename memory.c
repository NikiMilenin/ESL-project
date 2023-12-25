#include "memory.h"

uint32_t mem_end = BOOTLOADER_START_ADDR;
uint32_t mem_size = NRF_DFU_APP_DATA_AREA_SIZE;

uint32_t* get_read_ptr()
{
    uint32_t* ptr = (uint32_t *)mem_end;
    ptr -= 3;
    uint32_t* mem_start = (uint32_t *)(mem_end - mem_size);
    while (mem_start < ptr)
    {
        if (*ptr !=  __UINT32_MAX__) {
            NRF_LOG_INFO("READING FROM: %p", ptr - 2);
            return ptr - 2;
        }
        ptr -= 1;
    }
    return mem_start;
}

uint32_t* get_write_ptr(uint32_t* value)
{
    uint32_t* ptr = (uint32_t *)mem_end;
    ptr -= 3;
    uint32_t * mem_start = (uint32_t *)(mem_end - mem_size);
    while (mem_start < ptr)
    {
        if (*ptr !=  __UINT32_MAX__) {
            if (nrfx_nvmc_word_writable_check((uint32_t)ptr, *value))
            {
                NRF_LOG_INFO("WRITING TO: %p", ptr);
                return ptr;
            }
            else
            {
                NRF_LOG_INFO("WRITING TO: %p", ptr);
                return ptr + 1;
            }
        }
        ptr -= 1;
    }
    clear_mem();
    return mem_start;
}

void clear_mem()
{
    uint32_t * mem_start = (uint32_t *)(mem_end - mem_size);
    for (int i = 0; i < 3; i++)
    {
        nrfx_nvmc_page_erase((uint32_t)(mem_start + i * CODE_PAGE_SIZE));
    }
}

void write_arr(uint32_t* arr, int size_of_arr)
{
    uint32_t* ptr = get_write_ptr(arr);
    for (int i = 0; i < size_of_arr; i++) {
        nrfx_nvmc_word_write((uint32_t)(ptr + i), arr[i]);
        while(!nrfx_nvmc_write_done_check()){}
    }
} 

void read_arr(uint32_t* arr, int size_of_arr)
{
    uint32_t* ptr = get_read_ptr();
    NRF_LOG_INFO("READING %d %d %d", *ptr, *(ptr + 1), *(ptr + 2));
    for (int i = 0; i < size_of_arr; i++) {
        arr[i] = *(ptr + i);
    }     
}