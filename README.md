# Projeto de Transmissor LoRa (TX) para IoT – Rascunho de Trabalho em Grupo

> **Este documento é apenas um apoio interno para a equipe finalizar a tarefa.**  
> Não é o README oficial do projeto, só um ponto de partida para vocês complementarem, revisarem e adaptarem como acharem melhor. 😉

---

## Visão Geral

Esse projeto é um transmissor LoRa (TX) feito em linguagem C, usando a plataforma **BitDogLab**, para a tarefa da Residência EmbarcaTech TIC37. A ideia é simples: ler dados de sensores e enviar via LoRa para um receptor (RX). O código já está funcionando com envio e recepção básicos - supostamente, já que não pude testar o módulo LoRa em si.

---

## Funcionalidades Já Implementadas

- **Leitura de sensores via I2C** (barramento separado `i2c0`).
- **Envio LoRa com feedback visual** via LEDs.
- **Display OLED** mostrando status, contadores e últimos dados.
- Dois modos de envio:
  - **BTN_A** → envia dados reais dos sensores.
  - **BTN_B** → envia uma mensagem fixa de teste.
- Após o envio, o sistema entra em **modo escuta (RX)** para tentar receber confirmação.
- Se receber algo, mostra no display e pisca o LED verde.

---

## O que ainda precisa ser refinado

- As **indicações visuais (LEDs)** ainda estão provisórias. As **cores e comportamentos precisam ser ajustados** pra fazer mais sentido com o que o sistema está fazendo (ex: vermelho para erro, verde para sucesso etc).
- O **layout do display OLED também está básico**. Dá pra deixar mais organizado, com melhor distribuição das informações e organização da tela.
- Mensagens e prints no console podem ser melhorados para debug e testes.

---

## ⚙️ Como o código está organizado

| Função                | O que faz |
|-----------------------|-----------|
| `init_gpio()`         | Configura LEDs e botões. |
| `init_spi()`          | Inicia SPI para o módulo LoRa. |
| `init_display_i2c()`  | I2C do display OLED. |
| `init_sensor_i2c()`   | I2C dos sensores. |
| `init_display()`      | Mostra tela de inicialização. |
| `update_display()`    | Atualiza o que está no display. |
| `send_sensor_data()`  | Lê os sensores e envia via LoRa. |
| `send_test_message()` | Envia mensagem "Test Message". |
| `check_received_messages()` | Verifica se chegou alguma resposta via LoRa. |
| `main()`              | Loop principal com lógica dos botões. |

---

## Configurações LoRa 

- Frequência: **915 MHz**
- Potência de transmissão: **20 dBm** (apesar de o documento da atividade sugerir 17 dBm)

- Dá pra trocar tudo isso

---

## Sobre as bibliotecas 

- As bibliotecas de periféricos (display, sensores, etc.) são **as que o professor já disponibilizou em atividades anteriores**.
- Optei por **não utilizar sensor "bpm280"** por que a atividade pede o envio de duas informações e o "aht20" já faz isso(humidade e temperatura) e achei que ia dificultar a tarefa já que só dá para usar uma porta I2C na placa. Mas se quiserem, é só alterar o código princiapl pra colocar o outro sensor com pressão. 
- A parte de **LoRa foi adaptada**: uni a estrutura original do `LoRa.h` do professor com a biblioteca que utilizamos na atividade da semana passada.

---

## Para o grupo

Esse código é só um esqueleto funcional, acho que não vou poder fazer parte da monitoria amanhã pois tenho médico no mesmo horário. 
Fiquem **à vontade para modificar qualquer parte**: nomes de funções, layout do display, cores dos LEDs, lógica, etc.

A ideia é que cada um complemente com o que falta:
- Propósito final da aplicação (automação? agricultura? rastreamento?)
- Comentários e limpeza do código
- Ficha de proposta e relatório
- Gravação do vídeo de até 10 minutos
- Descrição dos registradores do SX1276

Se alguém quiser ajuda pra entender como está funcionando ou tiver dificuldade pra ajustar algo, só chamar! 👍
