#ifndef BME280_H
#define BME280_H

#include <stdio.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_check.h>
#include <driver/i2c.h>

const char *TAG_BME280_H = "[BME280_H]";

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

typedef struct 
{
    //Trimming Params Temperatura
    uint16_t T1;
    int16_t T2;
    int16_t T3;
    
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

    //Trimming Params Umidade
    uint8_t H1;
    int16_t H2;
    uint8_t H3;
    int16_t H4;
    int16_t H5;
    int8_t H6;

    //I2c handle para comunicao i2c
    i2c_cmd_handle_t i2c_bme280_handle;

    //Id do sensor
    uint8_t device_id;

} bme280_t;

//  @brief Funcao para verificar a comunicacao com BME280
//
//  @param *dev Ponteiro da estrutura de dados do BME280
//  @param port Qual dos canais I2C esta o sensor [I2C_PORT_NUM0 ... I2C_PORT_MAX-1]
//  @param addr Endereco do BME280
esp_err_t bme280_check(bme280_t *dev, i2c_port_t port, uint8_t addr);

esp_err_t bme280_get_trimming_params(bme280_t *dev);

#endif