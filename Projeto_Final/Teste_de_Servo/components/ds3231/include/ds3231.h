#ifndef DS3231_H
#define DS3231_H

/**
 *  @author Andre Menezes de Freitas Vale
 *  @version 1.0.0
 * 
 *  @brief Biblioteca para realizar a leitura do Relogio
 *  com o sensor DS3231
*/

#include <driver/i2c_master.h>

#define DS3231_ADDR     0x68

#define SECONDS         0x00
#define MINUTES         0x01
#define HOURS           0x02
#define DAY             0x03
#define DATE            0x04
#define MONTH_CENTURY   0x05
#define YEAR            0x06

#define ALARM_1_SEC     0x07
#define ALARM_1_MIN     0x08
#define ALARM_1_HOUR    0x09
#define ALARM_1_DAY_DATE 0x0A

#define ALARM_2_MIN     0x0B
#define ALARM_2_HOUR    0x0C
#define ALARM_2_DAY_DATE 0x0D

#define CONTROL         0x0E
#define CONTROL_STATUS  0x0F 

#define AGING_OFFSET    0x10

#define MSB_TEMP        0x11
#define LSB_TEMP        0x12

typedef struct
{
    uint8_t status;

    uint8_t temperaturaMSB;
    uint8_t temperaturaLSB;
    uint16_t Temperatura;

    uint8_t sec;           // [0..59]
    uint8_t min;           // [0..59]
    uint8_t hour;          // [0..12] ou [0..23], depende do bit 5 do registrador 0x02
    uint8_t day;           // [0..7]
    uint8_t date;          // [0..31]
    uint8_t month_century; // [0..12] (para o MONTH) + Century
    uint8_t year;          // [0..99]

    int Timeout;

    i2c_master_dev_handle_t dev_handle;

} ds3231_t;

//  @brief Verifica o bit de status do RTC
//
//  @param ds3231_handle Device Handle
//  @param *ds3231 Ponteiro da estrutura de dado do sensor
void ds3231_check_status(i2c_master_dev_handle_t ds3231_handle, ds3231_t *ds3231);

//  @brief Grava o valor do Relogio na estrutura de dados
//  do ds3231
//
//  @param ds3231_handle Device Handle
//  @param *ds3231 Ponteiro da estrutura de dado do sensor
//
//  @note Os valores estao formato BCD, ou seja, para ler corretamente
//  o dado deve converter para HEXADECIMAL.
void ds3231_get_clock(i2c_master_dev_handle_t ds3231_handle, ds3231_t *ds3231);

#endif