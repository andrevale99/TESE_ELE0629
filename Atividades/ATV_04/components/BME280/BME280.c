#include "BME280.h"

esp_err_t bme280_check(bme280_t *dev, i2c_port_t port, uint8_t addr)
{
    if (addr != BME280_ADDRESS_0 || addr != BME280_ADDRESS_1)
    {
        ESP_LOGE(TAG_BME280_H, "Endereco invalido");
        return ESP_ERR_NOT_ALLOWED;
    }

    dev->i2c_bme280_handle = i2c_cmd_link_create();
    i2c_master_start(dev->i2c_bme280_handle);
    i2c_master_write(dev->i2c_bme280_handle, (addr<<1) | I2C_MASTER_WRITE, sizeof(uint8_t), ACK);
    i2c_master_stop(dev->i2c_bme280_handle);

    esp_err_t teste = i2c_master_cmd_begin(port, dev->i2c_bme280_handle, pdMS_TO_TICKS(100));
    if(teste == ESP_FAIL)
    {
        ESP_LOGI(TAG_BME280_H, "BME280 NAO encontrado, verificar fiacao ou o sensor");
        return ESP_ERR_NOT_FOUND;
    }

    i2c_cmd_link_delete(dev->i2c_bme280_handle);    

    return ESP_OK;
}

esp_err_t bme280_get_trimming_params(bme280_t *dev)
{
    return ESP_OK;
}