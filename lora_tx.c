#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "rfm95.h"
#include "ssd1306.h"

// Definições de pinos - BitDogLab
#define PIN_RST   20
#define PIN_CS    17
#define PIN_IRQ   8

#define LED_TESTE     15
#define LED_AZUL      12
#define LED_VERDE     11
#define LED_VERMELHO  13

#define BTN_A     5
#define BTN_B     6

// Pinos I2C para display
#define I2C_SDA   14
#define I2C_SCL   15
#define I2C_PORT i2c1 
#define DISPLAY_ADDR 0x3C

// Variáveis globais
ssd1306_t display;
static char last_message[64] = "Aguardando...";
static char status_msg[32] = "PRONTO";
static int16_t last_rssi = 0;
static int8_t last_snr = 0;
static uint32_t msg_count = 0;
static uint32_t tx_count = 0;
static uint32_t rx_count = 0;

void init_gpio(void) {
    // LEDs
    gpio_init(LED_TESTE); gpio_set_dir(LED_TESTE, GPIO_OUT);
    gpio_init(LED_AZUL); gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_init(LED_VERDE); gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_init(LED_VERMELHO); gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    
    // Botões com pull-up
    gpio_init(BTN_A); gpio_set_dir(BTN_A, GPIO_IN); gpio_pull_up(BTN_A);
    gpio_init(BTN_B); gpio_set_dir(BTN_B, GPIO_IN); gpio_pull_up(BTN_B);
    
    // Estado inicial dos LEDs
    gpio_put(LED_AZUL, 1);  // Indica sistema operacional
    gpio_put(LED_VERDE, 0);
    gpio_put(LED_VERMELHO, 0);
    gpio_put(LED_TESTE, 0);
}

void init_spi(void) {
    // Configurar SPI0 para LoRa (BitDogLab usa SPI0)
    spi_init(spi0, 1000000); // 1 MHz
    gpio_set_function(16, GPIO_FUNC_SPI); // SCK
    gpio_set_function(19, GPIO_FUNC_SPI); // MOSI
    gpio_set_function(18, GPIO_FUNC_SPI); // MISO
}

void init_i2c(void) {
    // Configurar I2C para display
    i2c_init(I2C_PORT, 400000); // 400 kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

void init_display(void) {
    ssd1306_init(&display, 128, 64, false, DISPLAY_ADDR, I2C_PORT);
    ssd1306_config(&display);
    ssd1306_fill(&display, false);
    
    // Tela inicial
    ssd1306_draw_string(&display, "LoRa BitDogLab", 0, 0);
    ssd1306_draw_string(&display, "Freq: 915 MHz", 0, 8);
    ssd1306_draw_string(&display, "Power: 20 dBm", 0, 16);
    ssd1306_draw_string(&display, "Status: INIT", 0, 24);
    ssd1306_send_data(&display);
    
    sleep_ms(2000);
}

void update_display(void) {
    char temp[32];
    
    ssd1306_fill(&display, false);
    
    // Título
    ssd1306_draw_string(&display, "LoRa Transceiver", 0, 0);
    
    // Linha divisória
    ssd1306_hline(&display, 0, 127, 9, true);
    
    // Status
    snprintf(temp, sizeof(temp), "Status: %s", status_msg);
    ssd1306_draw_string(&display, temp, 0, 12);
    
    // Contadores
    snprintf(temp, sizeof(temp), "TX:%lu RX:%lu", tx_count, rx_count);
    ssd1306_draw_string(&display, temp, 0, 20);
    
    // Última mensagem
    ssd1306_draw_string(&display, "Ultima msg:", 0, 28);
    ssd1306_draw_string(&display, last_message, 0, 36);
    
    // RSSI e SNR se houver dados
    if (last_rssi != 0) {
        snprintf(temp, sizeof(temp), "RSSI:%ddBm SNR:%ddB", last_rssi, last_snr);
        ssd1306_draw_string(&display, temp, 0, 44);
    }
    
    // Instruções
    ssd1306_draw_string(&display, "A:Msg1  B:Msg2", 0, 56);
    
    ssd1306_send_data(&display);
}

void send_test_message(const char* message) {
    char full_msg[64];
    
    // Adicionar identificador da placa + contador à mensagem
    snprintf(full_msg, sizeof(full_msg), "P2:%s #%lu", message, ++msg_count);
    
    strcpy(status_msg, "ENVIANDO");
    update_display();
    
    gpio_put(LED_TESTE, 1);
    gpio_put(LED_VERDE, 1);
    
    printf("Enviando: %s\n", full_msg);
    rfm95_send_message(full_msg);
    
    tx_count++;
    strcpy(status_msg, "ENVIADO");
    strcpy(last_message, full_msg);
    
    sleep_ms(300);
    gpio_put(LED_TESTE, 0);
    gpio_put(LED_VERDE, 0);
    
    strcpy(status_msg, "ESCUTANDO");
    update_display();
}

void check_received_messages(void) {
    rfm95_packet_t packet;
    
    if (rfm95_available()) {
        if (rfm95_receive_message(&packet)) {
            gpio_put(LED_VERDE, 1);
            
            printf("Mensagem recebida: %s\n", packet.message);
            printf("RSSI: %d dBm, SNR: %d dB\n", packet.rssi, packet.snr);
            
            strcpy(last_message, packet.message);
            last_rssi = packet.rssi;
            last_snr = packet.snr;
            rx_count++;
            
            strcpy(status_msg, "RECEBIDO");
            update_display();
            
            sleep_ms(1000);
            gpio_put(LED_VERDE, 0);
            strcpy(status_msg, "ESCUTANDO");
        }
    }
}

void blink_status_led(void) {
    static uint32_t last_blink = 0;
    static bool led_state = false;
    
    uint32_t now = time_us_32() / 1000; // ms
    
    if (now - last_blink > 1000) { // Piscar a cada 1 segundo
        led_state = !led_state;
        gpio_put(LED_AZUL, led_state);
        last_blink = now;
    }
}

int main() {
    stdio_init_all();
    sleep_ms(2000); // Aguardar inicialização serial
    
    printf("=== LoRa BitDogLab Transceiver ===\n");
    printf("Iniciando sistema...\n");
    
    // Inicializar periféricos
    init_gpio();
    init_spi();
    init_i2c();
    init_display();
    printf("Inicializando LoRa...\n");
    
    // Inicializar e configurar LoRa
    rfm95_init(spi0, PIN_CS, PIN_RST, PIN_IRQ);
    rfm95_config(915.0, 20); // 915 MHz, 20 dBm
    
    printf("LoRa configurado - 915 MHz, 20 dBm\n");
    
    // Colocar em modo de recepção
    rfm95_set_mode_rx();
    strcpy(status_msg, "ESCUTANDO");
    
    printf("Sistema pronto! Aguardando comandos...\n");
    printf("Botão A: Enviar 'Hello World'\n");
    printf("Botão B: Enviar 'Test Message'\n");
    
    update_display();
    
    // Loop principal
    while (true) {
        // Verificar botões
        if (!gpio_get(BTN_A)) {
            send_test_message("Hello World");
            rfm95_set_mode_rx(); // Voltar para recepção
            sleep_ms(200); // Debounce
        }
        
        if (!gpio_get(BTN_B)) {
            send_test_message("Test Message");
            rfm95_set_mode_rx(); // Voltar para recepção
            sleep_ms(200); // Debounce  
        }
        
        // Verificar mensagens recebidas
        check_received_messages();
        
        // Piscar LED de status
        blink_status_led();
        
        // Atualizar display periodicamente
        static uint32_t last_display_update = 0;
        uint32_t now = time_us_32() / 1000;
        
        if (now - last_display_update > 5000) { // Atualizar a cada 5 segundos
            update_display();
            last_display_update = now;
        }
        
        sleep_ms(50); // Pequena pausa para não sobrecarregar
    }
    
    return 0;
}