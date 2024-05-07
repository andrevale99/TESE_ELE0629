#include "BMP280.h"

const char *TAG = "[BMP280_H]";

esp_err_t xError = ESP_FAIL;

const uint8_t len_buffer = 3;
uint8_t idx = 0;
uint8_t buffer[3];

void reset_buffer()
{
    for (idx = 0; idx < len_buffer; ++idx)
        buffer[idx] = 0;
}

void bmp280_set_timeout(bmp280_t *bmp280, int _timeout_ms)
{
    bmp280->Timeout = _timeout_ms;
}

void bmp280_get_trimming_params_temp(bmp280_t *bmp280, i2c_master_dev_handle_t dev_handle)
{
    reset_buffer();
    buffer[0] = 0x88;

    i2c_master_transmit_receive(dev_handle, &buffer[0], 1,
                                &buffer[1], 2, bmp280->Timeout);

    bmp280->T1 = (uint16_t)(((uint16_t)buffer[2] << 8) | buffer[1]);
    // ESP_LOGI(TAG, "Param T1 (%x | %x): %u", buffer[0], buffer[0] + 1,bmp280->T1);

    buffer[0] += 0x02;
    i2c_master_transmit_receive(dev_handle, &buffer[0], 1,
                                &buffer[1], 2, bmp280->Timeout);

    bmp280->T2 = (int16_t)(((int16_t)buffer[2] << 8) | buffer[1]);
    // ESP_LOGI(TAG, "Param T2 (%x | %x): %u", buffer[0], buffer[0] + 1,bmp280->T2);

    buffer[0] += 0x02;
    i2c_master_transmit_receive(dev_handle, &buffer[0], 1,
                                &buffer[1], 2, bmp280->Timeout);

    bmp280->T3 = (int16_t)(((int16_t)buffer[2] << 8) | buffer[1]);
    // ESP_LOGI(TAG, "Param T3 (%x | %x): %u", buffer[0], buffer[0] + 1,bmp280->T3);

    /*
        CRIAR LOGICA PARA PEGAR OS TRIMMINGS PARAMS
        DA PRESSAO
    */

    reset_buffer();
}

esp_err_t bmp280_set_mode(bmp280_t *bmp280, i2c_master_dev_handle_t dev_handle, uint8_t mode)
{
    reset_buffer();
    buffer[0] = BMP280_CTRL_MEAS;

    i2c_master_transmit_receive(dev_handle, &buffer[0], 1,
                                &buffer[1], 1, 100);

    // Limpa os bits para serem modificados
    // No endereco bmp280_CTRL_MEAS o MODE
    // representa os bits [1:0]
    buffer[1] &= ~(0x03);

    switch (mode)
    {
    case SLEEP:
        buffer[1] = SLEEP;
        xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
        break;

    case NORMAL:
        buffer[1] = NORMAL;
        xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
        break;

    case FORCED_1:
        buffer[1] = FORCED_1;
        xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
        break;

    case FORCED_2:
        buffer[1] = FORCED_2;
        xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
        break;

    default:
        ESP_LOGE(TAG, "Modo desconhecido");
        break;
    }

    return xError;
}

esp_err_t bmp280_set_standby(bmp280_t *bmp280, i2c_master_dev_handle_t dev_handle, uint8_t standby)
{
    reset_buffer();
    buffer[0] = BMP280_CONFIG;

    i2c_master_transmit_receive(dev_handle, &buffer[0], 1,
                                &buffer[1], 1, 100);

    // Limpa os bits para serem modificados
    // No endereco bmp280_CONFIG o STANDBY
    // representa os bits [7:5]
    buffer[1] &= ~((1 << 5) | ((1 << 6) | (1 << 7)));

    switch (standby)
    {
    case TSB0_5:
        buffer[1] |= (TSB0_5 << 5);
        xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
        break;

    case TSB62_5:
        buffer[1] |= (TSB62_5 << 5);
        xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
        break;

    case TSB125:
        buffer[1] |= (TSB125 << 5);
        xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
        break;

    case TSB250:
        buffer[1] |= (TSB250 << 5);
        xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
        break;

    case TSB500:
        buffer[1] |= (TSB500 << 5);
        xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
        break;

    case TSB1000:
        buffer[1] |= (TSB1000 << 5);
        xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
        break;

    case TSB10:
        buffer[1] |= (TSB10 << 5);
        xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
        break;

    case TSB20:
        buffer[1] |= (TSB20 << 5);
        xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
        break;

    default:
        ESP_LOGE(TAG, "Modo desconhecido");
        break;
    }

    return xError;
}

esp_err_t bmp280_set_oversamplig(bmp280_t *bmp280, i2c_master_dev_handle_t dev_handle, uint8_t osrs_p, uint8_t osrs_t)
{
    reset_buffer();

    uint8_t aux_temp[] = {osrs_p, osrs_t};
    const uint8_t len_aux_temp = 2;
    uint8_t deslocamento = 0;

    for (idx = 0; idx < len_aux_temp; idx++)
    {
        switch (idx)
        {
        case 0:
            deslocamento = 2;
            buffer[0] = BMP280_CTRL_MEAS;
            break;

        case 1:
            deslocamento = 5;
            buffer[0] = BMP280_CTRL_MEAS;
            break;

        default:
            break;
        }

        i2c_master_transmit(dev_handle, &buffer[0], 1, 100);

        buffer[1] &= ~(0x03 << deslocamento);

        switch (aux_temp[idx])
        {
        case SKIP:
            buffer[1] |= (SKIP << deslocamento);
            xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
            break;

        case x1:
            buffer[1] |= (x1 << deslocamento);
            xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
            break;

        case x2:
            buffer[1] |= (x2 << deslocamento);
            xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
            break;

        case x4:
            buffer[1] |= (x4 << deslocamento);
            xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
            break;

        case x8:
            buffer[1] |= (x8 << deslocamento);
            xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
            break;

        case x16:
            buffer[1] |= (x16 << deslocamento);
            xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
            break;

        default:
            ESP_LOGW(TAG, "Oversamplig Colocado em x16");
            buffer[1] |= (x16 << deslocamento);
            xError = i2c_master_transmit(dev_handle, &buffer[0], 2, bmp280->Timeout);
            break;
        }

        i2c_master_transmit_receive(dev_handle, &buffer[0], 1, &buffer[1], 1, 100);
    }

    return xError;
}

bool bmp280_is_measuring(bmp280_t *bmp280, i2c_master_dev_handle_t dev_handle)
{
    reset_buffer();
    buffer[0] = BMP280_STATUS;

    i2c_master_transmit_receive(dev_handle, &buffer[0], 1,
                                &buffer[1], 1, 100);

    uint8_t measuring = buffer[1] & (1 << 3);

    return measuring;
}

void bmp280_get_adc_T_P(bmp280_t *bmp280, i2c_master_dev_handle_t dev_handle)
{
    reset_buffer();

    buffer[0] = BMP280_TEMP_MSB;
    i2c_master_transmit_receive(dev_handle, &buffer[0], 1, &buffer[0], 3, bmp280->Timeout);

    bmp280->adc_T = ((uint32_t)buffer[0] << 12) | ((uint32_t)buffer[1] << 4) | ((uint32_t)buffer[1] >> 4);

    buffer[0] = BMP280_PRESS_MSB;
    i2c_master_transmit_receive(dev_handle, &buffer[0], 1, &buffer[0], 3, bmp280->Timeout);

    bmp280->adc_P = ((uint32_t)buffer[0] << 12) | ((uint32_t)buffer[1] << 4) | ((uint32_t)buffer[1] >> 4);
}

void bmp280_get_compesate_temperature(bmp280_t *bmp280, int32_t adc_T, int32_t *fine_temp)
{
    int32_t var1 = 0, var2 = 0;
    int32_t dig_T1 = bmp280->T1;
    int32_t dig_T2 = bmp280->T2;
    int32_t dig_T3 = bmp280->T3;

    var1 = ((((adc_T >> 3) - (dig_T1 << 1))) * (dig_T2)) >> 11;

    var2 = (((((adc_T >> 4) - (dig_T1)) * ((adc_T >> 4) - (dig_T1))) >> 12) * (dig_T3)) >> 14;

    *fine_temp = var1 + var2;

    bmp280->Temperature = (*fine_temp * 5 + 128) >> 8;
}
