#ifndef bmp280_H
#define bmp280_H

#include <stdio.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_check.h>

#include <driver/i2c_master.h>

#define ACK 0x1
#define NACK 0x0

//  @brief Endereco do bmp280
//
//  @note Quando SD0 estiver em 0 (Low)
#define BMP280_ADDRESS_0  0x76 

//  @brief Endereco do BMP280
//
//  @note Quando SD0 estiver em 1 (High)
#define BMP280_ADDRESS_1  0x77

//  @brief The “id” register contains the chip
//  identification number chip_id[7:0], which is 0x60
#define BMP280_DEVICE_ID 0xD0 

//  @brief The “reset” register contains the soft reset word reset[7:0].
//  If the value 0xB6 is written to the register,
//  the device is reset using the complete power-on-reset procedure.
//
//  @note Writing other values than 0xB6 has
//  no effect. The readout value is always 0x00.
#define BMP280_RESET 0xE0

//  @brief The “status” register contains 
//  two bits which indicate the status of the device.
#define BMP280_STATUS 0XF3

//  @brief The “ctrl_meas” register sets the pressure and 
//  temperature data acquisition options of the device.
//
//  @note The register needs to be written after changing 
//  “ctrl_hum” for the changes to become effective.
#define BMP280_CTRL_MEAS 0xF4

//  @brief The “config” register sets the rate, filter 
//  and interface options of the device
//  
//  @note Writes to the “config” register in normal mode may be ignored.
//  In sleep mode writes are not ignored
#define BMP280_CONFIG 0xF5

//  @brief The “press” register contains the raw pressure measurement output data up[19:0]
//
//  @param BMP280_PRESS_MSB press_msb[7:0]
#define BMP280_PRESS_MSB 0xF7

//  @brief The “press” register contains the raw pressure measurement output data up[19:0]
//
//  @param BMP280_PRESS_LSB press_lsb[7:0]
#define BMP280_PRESS_LSB 0xF8

//  @brief The “press” register contains the raw pressure measurement output data up[19:0]
//
//  @param  BMP280_PRESS_XLSB press_xlsb[3:0]
#define BMP280_PRESS_XLSB 0xF9

//  @brief The “temp” register contains the raw temperature measurement output data ut[19:0]  
//
//  @param BMP280_TEMP_MSB temp_msb[7:0]
#define BMP280_TEMP_MSB 0xFA

//  @brief The “temp” register contains the raw temperature measurement output data ut[19:0]  
//
//  @param BMP280_TEMP_LSB temp_lsb[7:0]
#define BMP280_TEMP_LSB 0xFB

//  @brief The “temp” register contains the raw temperature measurement output data ut[19:0]  
//
//  @param  BMP280_TEMP_XLSB temp_xlsb[3:0]
#define BMP280_TEMP_XLSB 0xFC

#define BMP280_MODE_SLEEP 0x0
#define BMP280_MODE_FORCED_1 0x1
#define BMP280_MODE_FORCED_2 0x2
#define BMP280_MODE_NORMAL 0x3

enum MODE
{
    SLEEP = 0b00,
    FORCED_1 = 0b01,
    FORCED_2 = 0b10,
    NORMAL = 0b11
};

enum STANDBY
{
    TSB0_5 = 0b000,
    TSB62_5 = 0b001,
    TSB125 = 0b010,
    TSB250 = 0b011,
    TSB500 = 0b100,
    TSB1000 = 0b101,
    TSB10 = 0b110,
    TSB20 = 0b111
};

enum OVERSAMPLING
{
    SKIP = 0b000,
    x1 = 0b001,
    x2 = 0b010,
    x4 = 0b011,
    x8 = 0b100,
    x16 = 0b101
};

typedef struct 
{
    //Trimming Params Temperatura
    uint16_t T1;
    int16_t T2;
    int16_t T3;

    int32_t adc_T;
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

    int32_t adc_P;
    uint32_t Pressure;

    //Variavel para espera de mensagem
    int Timeout;

} bmp280_t;

//  @brief Confiura o tempo em ms para espera na porta i2c
//
//  @param *bmp280 Endereco da estrutura bmp280_t
//  @param _timeout Tempo de espera em ms
//
//  @note Caso o Timeout seja -1, o tempo sera infinito
void bmp280_set_timeout(bmp280_t *bmp280, int _timeout_ms);

//  @brief Funcao para pegar os dados de calibracao 
//  para a Temperatura
//
//  @param *bmp280 Endereco da estrutura bmp280_t
//  @param dev_handle copia do i2c_master_dev_handle_t
void bmp280_get_trimming_params_temp(bmp280_t *bmp280, i2c_master_dev_handle_t dev_handle);

//  @brief Seleciona o Modo do Sensor
//
//  @param *bmp280 Endereco da estrutura bmp280_t
//  @param dev_handle copia do i2c_master_dev_handle_t
//  @param mode modo de operacao do sensor
//
//  @return ESP_XX Retorna Algo do tipo ESP_OK, ESP_FAIL, ...
esp_err_t bmp280_set_mode(bmp280_t *bmp280, i2c_master_dev_handle_t dev_handle, uint8_t mode);

//  @brief Configura o tempo de espera entre as conversoes
//
//  @param *bmp280 Endereco da estrutura bmp280_t
//  @param dev_handle copia do i2c_master_dev_handle_t
//  @param standby tempo de standby do sensor
//
//  @return ESP_XX Retorna Algo do tipo ESP_OK, ESP_FAIL, ...
esp_err_t bmp280_set_standby(bmp280_t *bmp280, i2c_master_dev_handle_t dev_handle, uint8_t standby);

//  @brief Seleciona o Oversamplig dos dados
//
//  @param *bmp280 Endereco da estrutura bmp280_t
//  @param dev_handle copia do i2c_master_dev_handle_t
//  @param osrs_p oversamplig da pressao
//  @param osrs_t oversamplig da temperatura
//
//  @return ESP_XX Retorna Algo do tipo ESP_OK, ESP_FAIL, ...
esp_err_t bmp280_set_oversamplig(bmp280_t *bmp280, i2c_master_dev_handle_t dev_handle, uint8_t osrs_p, uint8_t osrs_t);

//  @brief Verifica se esta realizando algum conversao
//
//  @param *bmp280 Endereco da estrutura bmp280_t
//  @param dev_handle copia do i2c_master_dev_handle_t
//
//  @return 0x1 Caso esteja convertendo
//  @return 0x0 Caso o resultado ja foi transferido para os registradores
bool bmp280_is_measuring(bmp280_t *bmp280, i2c_master_dev_handle_t dev_handle);

//  @brief Grava os valores do ADC do sensor na estrutura
//
//  @param *bmp280 Endereco da estrutura bmp280_t
//  @param dev_handle copia do i2c_master_dev_handle_t
//
//  @return 0x1 Caso esteja convertendo
//  @return 0x0 Caso o resultado ja foi transferido para os registradores
void bmp280_get_adc_T_P(bmp280_t *bmp280, i2c_master_dev_handle_t dev_handle);

//  @brief faz a leitura da temperatura
//
//  @param *bmp280 Endereco da estrutura bmp280_t
//  @param adc_T Leitura dos registradores de Temperatura do bmp280 (0xFA...0xFC)
//  @param *fine_temp Valor global da temperatura
void bmp280_get_compesate_temperature(bmp280_t *bmp280, int32_t adc_T, int32_t *fine_temp);

#endif
