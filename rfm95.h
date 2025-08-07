#ifndef RFM95_H
#define RFM95_H

#include "hardware/spi.h"
#include <stdint.h>
#include <stdbool.h>
#include "lora.h" 

// Estrutura para dados recebidos
typedef struct {
    char message[64];
    int16_t rssi;
    int8_t snr;
    uint8_t length;
    bool valid;
} rfm95_packet_t;

// Funções públicas
void rfm95_init(spi_inst_t *spi, uint cs, uint rst, uint irq);
void rfm95_config(float freq, int tx_power);
void rfm95_send_message(const char *msg);
bool rfm95_receive_message(rfm95_packet_t *packet);
void rfm95_set_mode_rx(void);
void rfm95_set_mode_tx(void);
void rfm95_set_mode_standby(void);
bool rfm95_available(void);
int16_t rfm95_get_rssi(void);
int8_t rfm95_get_snr(void);

#endif
