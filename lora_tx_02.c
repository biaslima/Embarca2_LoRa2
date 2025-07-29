/* EMBARCATECH - INTRODUÇÃO AO PROTOCOLO LORA - CAPÍTULO 3 / PARTE 4
 * BitDogLAB - Transmissor LoRa (TX)
 * Comunicação com módulo LoRa (Heltec WiFi LoRa 32 V3.2) via protocolo UART para transmissão de informações.
 * Prof: Ricardo Prates
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#define UART_ID uart1    // Seleciona a UART
#define BAUD_RATE 115200 // Define a taxa de transmissão
#define UART_TX_PIN 8    // Pino GPIO usado para TX
#define UART_RX_PIN 9    // Pino GPIO usado para RX
#define LED_PIN 13       // Pino GPIO usado para o LED

// Variáveis globais
static volatile uint16_t counter = 0;   // Variável inteira para contagem
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)
static volatile bool flag_var = 0;      // Variável de flag para debouncing

// Função para simular ps dados de um sensor de temperatura
float lerSensor() {
    // Gera um valor aleatório entre 200 e 300
    return 20.0 + (float)(rand() % 101) / 10.0;
}

// Função principal
int main()
{
    // Inicializa a biblioteca padrão
    stdio_init_all();

    // Inicializa a UART
    uart_init(UART_ID, BAUD_RATE);

    // Configura os pinos GPIO para a UART
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); // Configura o pino 0 para TX
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); // Configura o pino 1 para RX

    // Configura o pino do LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // string mensagem
    char message[20];

    // Loop principal
    while (1)
    {
        //Lê os dados do sensor
        float temperatura = lerSensor();

        // Formata e armazena uma string
        sprintf(message, "%0.2f", temperatura);

        // Envia a mensagem pela UART
        uart_puts(UART_ID, message);

        // Exibe a mensagem no serial monitor
        //printf("data: %d\n", data);
        printf("Mensagem enviada: %s\n", message);

        // Pisca o LED
        gpio_put(LED_PIN, 1);
        sleep_ms(50);
        gpio_put(LED_PIN, 0);
        sleep_ms(50);

        // Pequeno atraso para evitar sobrecarga no loop principal
        sleep_ms(3000);
            
    }

    return 0;
}
