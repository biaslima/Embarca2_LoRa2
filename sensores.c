#include "sensores.h"
#include "aht20.h"
#include "bmp280.h"
#include <stdio.h>

static i2c_inst_t *i2c_usado = NULL;

void sensores_init(i2c_inst_t *i2c) {
    i2c_usado = i2c;

    // Inicializar sensores
    aht20_init(i2c_usado);
    bmp280_init(i2c_usado);
}

void sensores_ler(char *out_str, size_t len) {
    AHT20_Data dados;
    float press_bmp = 0.0f;
    float temp_bmp = 0.0f;

    // Leitura do AHT20
    if (!aht20_read(i2c_usado, &dados)) {
        dados.temperature = -1.0f;
        dados.humidity = -1.0f;
    }

    // Leitura do BMP280 usando as novas funções
    temp_bmp = bmp280_read_temperature();
    press_bmp = bmp280_read_pressure();

    // Formatar a mensagem
    snprintf(out_str, len, "T_AHT=%.1fC U=%.1f%% P_BMP=%.1fkPa T_BMP=%.1fC", dados.temperature, dados.humidity, press_bmp, temp_bmp);
}