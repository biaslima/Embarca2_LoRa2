#ifndef RFM95_H
#define RFM95_H

#include "hardware/spi.h"
#include <stdint.h>
#include <stdbool.h>

// Registradores RFM95
#define RFM95_REG_FIFO                 0x00
#define RFM95_REG_OP_MODE              0x01
#define RFM95_REG_FRF_MSB              0x06
#define RFM95_REG_FRF_MID              0x07
#define RFM95_REG_FRF_LSB              0x08
#define RFM95_REG_PA_CONFIG            0x09
#define RFM95_REG_FIFO_ADDR_PTR        0x0D
#define RFM95_REG_FIFO_TX_BASE_ADDR    0x0E
#define RFM95_REG_FIFO_RX_BASE_ADDR    0x0F
#define RFM95_REG_FIFO_RX_CURRENT_ADDR 0x10
#define RFM95_REG_IRQ_FLAGS            0x12
#define RFM95_REG_RX_NB_BYTES          0x13
#define RFM95_REG_PKT_SNR_VALUE        0x19
#define RFM95_REG_PKT_RSSI_VALUE       0x1A
#define RFM95_REG_MODEM_CONFIG1        0x1D
#define RFM95_REG_MODEM_CONFIG2        0x1E
#define RFM95_REG_PREAMBLE_MSB         0x20
#define RFM95_REG_PREAMBLE_LSB         0x21
#define RFM95_REG_PAYLOAD_LENGTH       0x22
#define RFM95_REG_MODEM_CONFIG3        0x26
#define RFM95_REG_DIO_MAPPING1         0x40

// Modos de operação
#define RFM95_MODE_SLEEP               0x00
#define RFM95_MODE_STDBY               0x01
#define RFM95_MODE_TX                  0x03
#define RFM95_MODE_RXCONTINUOUS        0x05
#define RFM95_MODE_CAD                 0x07
#define RFM95_LONG_RANGE_MODE          0x80

// Flags de interrupção
#define RFM95_IRQ_TX_DONE              0x08
#define RFM95_IRQ_RX_DONE              0x40
#define RFM95_IRQ_CAD_DONE             0x04
#define RFM95_IRQ_CAD_DETECTED         0x01

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
