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
#define LED_VERMELHO 28
#define LED_VERDE 22
#define LED_AZUL 18

// Mapeamento das teclas do Keypad
const char teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Variável para armazenar o LED atualmente aceso
int led_atual = -1;

/**
 * @brief Configura os GPIOs para o teclado e LEDs.
 */
void configurar_gpio() {
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
    gpio_init(LED_VERMELHO);
    gpio_init(LED_VERDE);
    gpio_init(LED_AZUL);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_set_dir(LED_AZUL, GPIO_OUT);

    // Inicialmente, desligar todos os LEDs
    gpio_put(LED_VERMELHO, 0);
    gpio_put(LED_VERDE, 0);
    gpio_put(LED_AZUL, 0);
}

/**
 * @brief Verifica qual tecla do teclado matricial foi pressionada.
 * 
 * @return char A tecla pressionada ou '\0' se nenhuma tecla for pressionada.
 */
char escanear_teclado() {
    // Iterando pelas linhas
    for (int linha = 0; linha < 4; linha++) {
        // Definir a linha atual como LOW e as outras como HIGH
        gpio_put(ROW1, linha == 0 ? 0 : 1);
        gpio_put(ROW2, linha == 1 ? 0 : 1);
        gpio_put(ROW3, linha == 2 ? 0 : 1);
        gpio_put(ROW4, linha == 3 ? 0 : 1);

        // Verificar cada coluna
        if (!gpio_get(COL1)) return teclas[linha][0];
        if (!gpio_get(COL2)) return teclas[linha][1];
        if (!gpio_get(COL3)) return teclas[linha][2];
        if (!gpio_get(COL4)) return teclas[linha][3];
    }
    // Retornar '\0' se nenhuma tecla for pressionada
    return '\0';
}

/**
 * @brief Controla os LEDs com base na tecla pressionada.
 * 
 * @param tecla A tecla pressionada.
 */
void controlar_led(char tecla) {
    // Desligar o LED atualmente aceso, se houver
    if (led_atual != -1) {
        gpio_put(led_atual, 0);
        led_atual = -1;
    }

    // Acender o LED correspondente à tecla pressionada
    switch (tecla) {
        case '1':
            gpio_put(LED_VERMELHO, 1);
            led_atual = LED_VERMELHO;
            printf("Tecla 1 pressionada, LED vermelho ligado.\n");
            break;
        case '2':
            gpio_put(LED_VERDE, 1);
            led_atual = LED_VERDE;
            printf("Tecla 2 pressionada, LED verde ligado.\n");
            break;
        case '3':
            gpio_put(LED_AZUL, 1);
            led_atual = LED_AZUL;
            printf("Tecla 3 pressionada, LED azul ligado.\n");
            break;
        default:
            printf("Tecla %c pressionada, todos os LEDs desligados.\n", tecla);
            break;
    }
}

int main() {
    // Inicializar o sistema padrão e configurar GPIOs
    stdio_init_all();
    configurar_gpio();

    // Instruções iniciais sobre o programa
    printf("Essa simulação controla os LEDs RGB usando um teclado matricial 4x4.\n");
    printf("Aperte as seguintes teclas para controlar os LEDs:\n");
    printf("  - Tecla '1': Acende o LED vermelho.\n");
    printf("  - Tecla '2': Acende o LED verde.\n");
    printf("  - Tecla '3': Acende o LED azul.\n");
    printf("Qualquer outra tecla irá apagar todos os LEDs.\n");

    while (true) {
        char tecla = escanear_teclado();
        controlar_led(tecla);
        sleep_ms(100);
    }
}