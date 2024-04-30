#include "BME280.h"

const char *TAG = "[BME280_H]";

void bme280_set_timeout(bme280_t *bme280, int _timeout_ms)
{
    bme280->Timeout = _timeout_ms;
}

void bme280_get_trimming_params_temp(bme280_t *bme280, i2c_master_dev_handle_t dev_handle)
{
    uint8_t AddrTrimming[] = {0x88, 0x89};    
    uint8_t aux[] = {0,0};

    i2c_master_transmit_receive(dev_handle, &AddrTrimming, 2, 
                                &aux, 2, bme280->Timeout);

    bme280->T1 = (uint16_t)(aux[1] << 8) | aux[0];
    ESP_LOGI(TAG, "Param T1: %u", bme280->T1);

    AddrTrimming[0] += 0x02;
    AddrTrimming[1] += 0x02;
    i2c_master_transmit_receive(dev_handle, &AddrTrimming, 2, 
                                &aux, 2, bme280->Timeout);
    
    bme280->T2 = (uint16_t)(aux[1] << 8) | aux[0];    
    ESP_LOGI(TAG, "Param T2: %i", bme280->T2);
    
    AddrTrimming[0] += 0x02;
    AddrTrimming[1] += 0x02;
    i2c_master_transmit_receive(dev_handle, &AddrTrimming, 2, 
                                &aux, 2, bme280->Timeout);
    
    bme280->T3 = (uint16_t)(aux[1] << 8) | aux[0];    
    ESP_LOGI(TAG, "Param T3: %i", bme280->T3);
}

void bme280_get_temperature(bme280_t *bme280, int32_t adc_T, int32_t *fine_temp)
{
    int32_t var1 = 0, var2 = 0;

    var1 =  ((((adc_T>>3) - ((int32_t)bme280->T1 << 1))) * ((int32_t)bme280->T2)) >> 11;

    var2 = ((((adc_T>>4) - ((int32_t)bme280->T1)) * ((adc_T >> 4) - ((int32_t)bme280->T1)) >> 12) * ((int32_t)bme280->T3)) >> 14;

    *fine_temp = var1 + var2;

    bme280->Temperature = (*fine_temp * 5 + 128) >> 8;
}
