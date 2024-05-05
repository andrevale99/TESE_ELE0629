#ifndef BME280_H
#define BME280_H

#include <stdio.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_check.h>

#include <driver/i2c_master.h>

#define ACK 0x1
#define NACK 0x0

//  @brief Endereco do BME280
//
//  @note Quando SD0 estiver em 0 (Low)
#define BME280_ADDRESS_0  0x76 

//  @brief Endereco do BME280
//
//  @note Quando SD0 estiver em 1 (High)
#define BME280_ADDRESS_1  0x77

//  @brief The “id” register contains the chip
//  identification number chip_id[7:0], which is 0x60
#define BME280_DEVICE_ID 0xD0 

//  @brief The “reset” register contains the soft reset word reset[7:0].
//  If the value 0xB6 is written to the register,
//  the device is reset using the complete power-on-reset procedure.
//
//  @note Writing other values than 0xB6 has
//  no effect. The readout value is always 0x00.
#define BME280_RESET 0xE0

//  @brief The “ctrl_hum” register sets
//  the humidity data acquisition options of the device. 
//
//  @note Changes to this register only become effective
//  after a write operation to “ctrl_meas”.
#define BME280_CTRL_HUM 0xF2

//  @brief The “status” register contains 
//  two bits which indicate the status of the device.
#define BME280_STATUS 0XF3

//  @brief The “ctrl_meas” register sets the pressure and 
//  temperature data acquisition options of the device.
//
//  @note The register needs to be written after changing 
//  “ctrl_hum” for the changes to become effective.
#define BME280_CTRL_MEAS 0xF4

//  @brief The “config” register sets the rate, filter 
//  and interface options of the device
//  
//  @note Writes to the “config” register in normal mode may be ignored.
//  In sleep mode writes are not ignored
#define BME280_CONFIG 0xF5

//  @brief The “press” register contains the raw pressure measurement output data up[19:0]
//
//  @param BME280_PRESS_MSB press_msb[7:0]
#define BME280_PRESS_MSB 0xF7

//  @brief The “press” register contains the raw pressure measurement output data up[19:0]
//
//  @param BME280_PRESS_LSB press_lsb[7:0]
#define BME280_PRESS_LSB 0xF8

//  @brief The “press” register contains the raw pressure measurement output data up[19:0]
//
//  @param  BME280_PRESS_XLSB press_xlsb[3:0]
#define BME280_PRESS_XLSB 0xF9

//  @brief The “temp” register contains the raw temperature measurement output data ut[19:0]  
//
//  @param BME280_TEMP_MSB temp_msb[7:0]
#define BME280_TEMP_MSB 0xFA

//  @brief The “temp” register contains the raw temperature measurement output data ut[19:0]  
//
//  @param BME280_TEMP_LSB temp_lsb[7:0]
#define BME280_TEMP_LSB 0xFB

//  @brief The “temp” register contains the raw temperature measurement output data ut[19:0]  
//
//  @param  BME280_TEMP_XLSB temp_xlsb[3:0]
#define BME280_TEMP_XLSB 0xFC

//  @brief The “temp” register contains the raw temperature measurement output data ut[19:0]
//
//  @param BME280_HUM_MSB hum_msb[7:0]
#define BME280_HUM_MSB 0xFD

//  @brief The “temp” register contains the raw temperature measurement output data ut[19:0]
//
//  @param BME280_HUM_LSB hum_lsb[7:0]
#define BME280_HUM_LSB 0xFE

#define BME280_MODE_SLEEP 0x0
#define BME280_MODE_FORCED_1 0x1
#define BME280_MODE_FORCED_2 0x2
#define BME280_MODE_NORMAL 0x3

typedef struct 
{
    //Trimming Params Temperatura
    uint16_t T1;
    int16_t T2;
    int16_t T3;

    int32_t Temperature;
    
    //Trimming Params Pressao
    uint16_t P1;
    int16_t P2;
    int16_t P3;
    int16_t P4;
    int16_t P5;
    int16_t P6;
    int16_t P7;
    int16_t P8;
    int16_t P9;

    uint32_t Pressure;

    //Trimming Params Umidade
    uint8_t H1;
    int16_t H2;
    uint8_t H3;
    int16_t H4;
    int16_t H5;
    int8_t H6;

    uint32_t Humidity;

    //Variavel para espera de mensagem
    int Timeout;

    //Id do sensor
    uint8_t device_id;

    //Mode
    uint8_t mode;

} bme280_t;

//  @brief Confiura o tempo em ms para espera na porta i2c
//
//  @param *bme280 Endereco da estrutura bme280_t
//  @param _timeout Tempo de espera em ms
//
//  @note Caso o Timeout seja -1, o tempo sera infinito
void bme280_set_timeout(bme280_t *bme280, int _timeout_ms);

//  @brief Funcao para pegar os dados de calibracao 
//  para a Temperatura
//
//  @param *bme280 Endereco da estrutura bme280_t
//  @param dev_handle copia do i2c_master_dev_handle_t
//
//  @return ESP_OK Caso todos os parametros foram gravados
//  @return ESP_ERR_TIMEOUT Caso o tempo de espera tenha estourado
//  @return ESP_ERR_INVALID_ARG Erro em algum parametro
void bme280_get_trimming_params_temp(bme280_t *bme280, i2c_master_dev_handle_t dev_handle);

esp_err_t bme280_set_mode(bme280_t *bme280, i2c_master_dev_handle_t dev_handle, uint8_t mode);

//  @brief faz a leitura da temperatura
//
//  @param *bme280 Endereco da estrutura bme280_t
//  @param adc_T Leitura dos registradores de Temperatura do BME280 (0xFA...0xFC)
//  @param *fine_temp Valor global da temperatura
void bme280_get_temperature(bme280_t *bme280, int32_t adc_T);

#endif
