#include "BME280.h"

const char *TAG = "[BME280_H]";

esp_err_t xError = ESP_FAIL;

void bme280_set_timeout(bme280_t *bme280, int _timeout_ms)
{
    bme280->Timeout = _timeout_ms;
}

void bme280_get_trimming_params_temp(bme280_t *bme280, i2c_master_dev_handle_t dev_handle)
{
    uint8_t AddrTrimming[] = {0x88, 0x89};    
    uint8_t aux[] = {0,0};

    i2c_master_transmit_receive(dev_handle, &AddrTrimming[0], 2, 
                                &aux, 2, bme280->Timeout);

    bme280->T1 = ((uint16_t)aux[1] << 8) | aux[0];
    ESP_LOGI(TAG, "Param T1: %u", bme280->T1);

    AddrTrimming[0] += 0x02;
    AddrTrimming[1] += 0x02;
    i2c_master_transmit_receive(dev_handle, &AddrTrimming[0], 2, 
                                &aux, 2, bme280->Timeout);
    
    bme280->T2 = ((uint16_t)aux[1] << 8) | aux[0];    
    ESP_LOGI(TAG, "Param T2: %i", bme280->T2);
    
    AddrTrimming[0] += 0x02;
    AddrTrimming[1] += 0x02;
    i2c_master_transmit_receive(dev_handle, &AddrTrimming[0], 2, 
                                &aux, 2, bme280->Timeout);
    
    bme280->T3 = ((uint16_t)aux[1] << 8) | aux[0];    
    ESP_LOGI(TAG, "Param T3: %i", bme280->T3);
}

esp_err_t bme280_set_mode(bme280_t *bme280, i2c_master_dev_handle_t dev_handle, uint8_t mode)
{
    uint8_t buffer[] = {BME280_CTRL_MEAS, mode};

    xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bme280->Timeout);

    return xError;
}

void bme280_get_temperature(bme280_t *bme280, int32_t adc_T)
{
    int32_t var1 = 0, var2 = 0;
    int32_t dig_T1 = bme280->T1;
    int32_t dig_T2 = bme280->T2;
    int32_t dig_T3 = bme280->T3;

    var1 =  ((((adc_T>>3) - (dig_T1<<1))) * (dig_T2)) >> 11;;

    var2 = (((((adc_T>>4) - (dig_T1)) * ((adc_T>>4) - (dig_T1))) >> 12) * (dig_T3)) >> 14;

    int32_t fine_temp = var1 + var2;

    bme280->Temperature = (fine_temp * 5 + 128) >> 8;
}
