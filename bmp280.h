#ifndef BMP280_H
#define BMP280_H

#include "hardware/i2c.h"
#include "pico/stdlib.h"

// Endereço do sensor
#define ADDR _u(0x76) // Usando 0x76 como endereço padrão, verifique a sua placa

// Definições de registradores
#define REG_CONFIG      0xF5
#define REG_CTRL_MEAS   0xF4
#define REG_RESET       0xE0
#define REG_ID          0xD0
#define REG_DIG_T1_LSB  0x88
#define REG_PRESSURE_MSB 0xF7

#define NUM_CALIB_PARAMS 24

struct bmp280_calib_param {
    uint16_t dig_t1;
    int16_t dig_t2;
    int16_t dig_t3;

    uint16_t dig_p1;
    int16_t dig_p2;
    int16_t dig_p3;
    int16_t dig_p4;
    int16_t dig_p5;
    int16_t dig_p6;
    int16_t dig_p7;
    int16_t dig_p8;
    int16_t dig_p9;
};

// Funções públicas
void bmp280_init(i2c_inst_t *i2c);
void bmp280_read_raw(i2c_inst_t *i2c, int32_t* temp, int32_t* pressure);
void bmp280_reset(i2c_inst_t *i2c);
void bmp280_get_calib_params(i2c_inst_t *i2c, struct bmp280_calib_param* params);

// Novas funções para ler e converter os dados
float bmp280_read_temperature();
float bmp280_read_pressure();

#endif
