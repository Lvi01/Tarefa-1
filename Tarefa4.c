#include <stdio.h>
#include "pico/stdlib.h"

// Definição dos pinos do Keypad
#define ROW1 5
#define ROW2 6
#define ROW3 7
#define ROW4 8
#define COL1 1
#define COL2 2
#define COL3 3
#define COL4 4

// Definição dos pinos dos LEDs
#define LED_RED 28
#define LED_GREEN 22
#define LED_BLUE 18

// Definição do pino do Buzzer
#define BUZZER 10

// Mapeamento das teclas do Keypad
const char keys[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Variável para armazenar o LED atualmente aceso
int current_led = -1;

// Função para configurar os GPIOs
void setup_gpio() {
    // Configurando as linhas (ROW) como saídas
    gpio_init(ROW1);
    gpio_init(ROW2);
    gpio_init(ROW3);
    gpio_init(ROW4);
    gpio_set_dir(ROW1, GPIO_OUT);
    gpio_set_dir(ROW2, GPIO_OUT);
    gpio_set_dir(ROW3, GPIO_OUT);
    gpio_set_dir(ROW4, GPIO_OUT);

    // Configurando as colunas (COL) como entradas com pull-up
    gpio_init(COL1);
    gpio_init(COL2);
    gpio_init(COL3);
    gpio_init(COL4);
    gpio_set_dir(COL1, GPIO_IN);
    gpio_set_dir(COL2, GPIO_IN);
    gpio_set_dir(COL3, GPIO_IN);
    gpio_set_dir(COL4, GPIO_IN);
    gpio_pull_up(COL1);
    gpio_pull_up(COL2);
    gpio_pull_up(COL3);
    gpio_pull_up(COL4);

    // Configurando os LEDs como saída
    gpio_init(LED_RED);
    gpio_init(LED_GREEN);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);

    // Inicialmente, desligar todos os LEDs
    gpio_put(LED_RED, 0);
    gpio_put(LED_GREEN, 0);
    gpio_put(LED_BLUE, 0);

    // Configurando o buzzer como saída
    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);
    gpio_put(BUZZER, 0); // Buzzer inicialmente desligado
}

// Função para verificar qual tecla foi pressionada
char scan_keypad() {
    // Iterando pelas linhas
    for (int row = 0; row < 4; row++) {
        // Definir a linha atual como LOW e as outras como HIGH
        gpio_put(ROW1, row == 0 ? 0 : 1);
        gpio_put(ROW2, row == 1 ? 0 : 1);
        gpio_put(ROW3, row == 2 ? 0 : 1);
        gpio_put(ROW4, row == 3 ? 0 : 1);

        // Verificar cada coluna
        if (!gpio_get(COL1)) return keys[row][0];
        if (!gpio_get(COL2)) return keys[row][1];
        if (!gpio_get(COL3)) return keys[row][2];
        if (!gpio_get(COL4)) return keys[row][3];
    }
    // Retornar '\0' se nenhuma tecla for pressionada
    return '\0';
}

// Função para controlar os LEDs e o buzzer
void control_device(char key) {
    // Desligar o LED atualmente aceso, se houver
    if (current_led != -1) {
        gpio_put(current_led, 0);
        current_led = -1;
    }

    // Controlar dispositivos com base na tecla pressionada
    switch (key) {
        case '1':
            gpio_put(LED_RED, 1);
            current_led = LED_RED;
            printf("Tecla 1 pressionada, LED vermelho ligado.\n");
            break;
        case '2':
            gpio_put(LED_GREEN, 1);
            current_led = LED_GREEN;
            printf("Tecla 2 pressionada, LED verde ligado.\n");
            break;
        case '3':
            gpio_put(LED_BLUE, 1);
            current_led = LED_BLUE;
            printf("Tecla 3 pressionada, LED azul ligado.\n");
            break;
        case 'B':
            gpio_put(BUZZER, 1);
            printf("Tecla B pressionada, buzzer ativado.\n");
            sleep_ms(2000); // Emitir som por 500 ms
            gpio_put(BUZZER, 0);
            printf("Buzzer desativado.\n");
            break;
        default:
            printf("Tecla %c pressionada, todos os LEDs desligados.\n", key);
            break;
    }
}

int main() {
    // Inicializar o sistema padrão e configurar GPIOs
    stdio_init_all();
    setup_gpio();

    // Instruções iniciais sobre o programa
    printf("Essa simulação controla os LEDs RGB e um buzzer usando um teclado matricial 4x4.\n");
    printf("Aperte as seguintes teclas para controlar os dispositivos:\n");
    printf("  - Tecla '1': Acende o LED vermelho.\n");
    printf("  - Tecla '2': Acende o LED verde.\n");
    printf("  - Tecla '3': Acende o LED azul.\n");
    printf("  - Tecla 'B': Emite som no buzzer.\n");
    printf("Qualquer outra tecla irá apagar todos os LEDs.\n");

    while (true) {
        char key = scan_keypad(); // Ler o keypad

        if (key != '\0') { // Se uma tecla foi pressionada
            control_device(key); // Controlar dispositivos com base na tecla

            // Aguardar a liberação da tecla
            while (scan_keypad() != '\0') {
                sleep_ms(10);
            }
        }

        sleep_ms(50); // Pequeno atraso para evitar leituras incorretas
    }
}
