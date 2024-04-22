#include "BME280.h"

const char *TAG = "[BME280_H]";

void bme280_set_timeout(bme280_t *bme280, int _timeout_ms)
{
    bme280->Timeout = _timeout_ms;
}

esp_err_t bme280_get_trimming_params(bme280_t *bme280, i2c_master_dev_handle_t dev_handle)
{
    uint8_t AddrTrimming = 0x88;

    bme280->err_bme = i2c_master_transmit_receive(dev_handle, &AddrTrimming, 1,
                                                &(bme280->T1), sizeof(uint16_t)*3, bme280->Timeout);

    ESP_ERROR_CHECK(bme280->err_bme);    

    AddrTrimming = 0x8E;

    bme280->err_bme = i2c_master_transmit_receive(dev_handle, &AddrTrimming, 1,
                                                &(bme280->P1), sizeof(uint16_t)*9, bme280->Timeout);
    
    ESP_ERROR_CHECK(bme280->err_bme);    

    return bme280->err_bme;
}

void bme280_get_temperature(bme280_t *bme280, int32_t adc_T, int32_t *fine_temp)
{
    int32_t var1 = 0, var2 = 0;

    var1 =  ((((adc_T>>3) - ((int32_t)bme280->T1 << 1))) * ((int32_t)bme280->T2)) >> 11;

    var2 = ((((adc_T>>4) - ((int32_t)bme280->T1)) * ((adc_T >> 4) - ((int32_t)bme280->T1)) >> 12) * ((int32_t)bme280->T3)) >> 14;

    *fine_temp = var1 + var2;

    bme280->Temperature = (*fine_temp * 5 + 128) >> 8;
}