#include <stdio.h>
#include "DS3231.h"

const char *TAG = "[DS3231_H]";

uint8_t ucWriteBuffer = 0;

void vDS3231_check_status(i2c_master_dev_handle_t ds3231_handle, ds3231_t *ds3231)
{
    ucWriteBuffer = CONTROL_STATUS;
    i2c_master_transmit_receive(ds3231_handle, &ucWriteBuffer, 1,
                                &(ds3231->status), 1, ds3231->Timeout);
}

void vDS3231_get_clock(i2c_master_dev_handle_t ds3231_handle, ds3231_t *ds3231)
{
    ucWriteBuffer = SECONDS;
    i2c_master_transmit_receive(ds3231_handle, &ucWriteBuffer, 1,
                                &(ds3231->sec), 3, ds3231->Timeout);
}