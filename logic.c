#include "logic.h"

void read_state(uint32_t* arr)
{
    read_arr(arr, 3);
    if (*(arr) > 360 * HSV_SCALER || *(arr + 1) > 100 * HSV_SCALER || *(arr + 2) > 100 * HSV_SCALER) {
        arr[0] = 360 * (DEVICE_ID % 100) * HSV_SCALER / 100; 
        arr[1] = 100 * HSV_SCALER;
        arr[2] = 100 * HSV_SCALER;
    }
}

void write_state(uint32_t* arr) 
{
    write_arr(arr, 3);
}