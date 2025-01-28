#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pio_matrix.pio.h"

#define MATRIX 7
#define NUM_PIXELS 25
#define BUTTON_A 5
#define BUTTON_B 6
#define DEBOUNCE_DELAY 200 // Em milissegundos
#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12

// Matriz de números para exibição
const double numbers[10][25] = {
    // Numero 0
    {0.0, 1.0, 1.0, 1.0, 0.0,
     1.0, 1.0, 0.0, 0.0, 1.0,
     1.0, 0.0, 1.0, 0.0, 1.0,
     1.0, 0.0, 0.0, 1.0, 1.0,
     0.0, 1.0, 1.0, 1.0, 0.0},

    // Numero 1
    {0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0},

    // Numero 2
    {0.0, 0.0, 1.0, 1.0, 0.0,
     1.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 1.0},

    // Numero 3
    {0.0, 1.0, 1.0, 1.0, 0.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     0.0, 0.0, 1.0, 1.0, 0.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     0.0, 1.0, 1.0, 1.0, 0.0},

    // Numero 4
    {0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 1.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0},

    // Numero 5
    {1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0},

    // Numero 6
    {1.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0},

    // Numero 7
    {1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, 0.0, 0.0, 0.0},

    // Numero 8
    {0.0, 1.0, 1.0, 1.0, 0.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     0.0, 1.0, 1.0, 1.0, 0.0},

    // Numero 9
    {1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0}};

volatile int current_number = 0;
volatile uint32_t last_debounce_time_a = 0;
volatile uint32_t last_debounce_time_b = 0;

void update_matrix(const double *pattern, PIO pio, uint sm)
{
    uint32_t value;
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        int inverted_index = NUM_PIXELS - 1 - i;
        value = ((uint32_t)(pattern[inverted_index] * 255) << 8);
        pio_sm_put_blocking(pio, sm, value);
    }
}

void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (gpio == BUTTON_A && (events & GPIO_IRQ_EDGE_FALL))
    {
        if (current_time - last_debounce_time_a > DEBOUNCE_DELAY)
        {
            last_debounce_time_a = current_time;
            if (current_number < 9)
            {
                current_number++;
            }
        }
    }
    else if (gpio == BUTTON_B && (events & GPIO_IRQ_EDGE_FALL))
    {
        if (current_time - last_debounce_time_b > DEBOUNCE_DELAY)
        {
            last_debounce_time_b = current_time;
            if (current_number > 0)
            {
                current_number--;
            }
        }
    }
}

int main()
{
    stdio_init_all();

    // Inicializa LEDs RGB
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);

    uint32_t last_blink_time = 0;
    bool led_state = false;

    // Configuração para controlar a matriz de LEDs
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, MATRIX);

    // Configuração dos Botões
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);

    while (true)
    {
        uint32_t current_time = to_ms_since_boot(get_absolute_time());

        if (current_time - last_blink_time >= 200)
        {
            last_blink_time = current_time;
            led_state = !led_state;
            gpio_put(LED_RED, led_state);
        }

        update_matrix(numbers[current_number], pio, sm);
        sleep_ms(100);
    }

    return 0;
}