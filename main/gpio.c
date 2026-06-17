#include "esp32_regs.h"

void gpio_init_output(int pin) {
    volatile uint32_t *mux = (volatile uint32_t *)(0x3FF49000 + pin * 4);
    *mux = (*mux & ~0x13) | (2 << 12);
    GPIO_ENABLE_W1TS = (1U << pin);
}

void gpio_set(int pin) {
    GPIO_OUT_W1TS_REG = (1U << pin);
}

void gpio_clear(int pin) {
    GPIO_OUT_W1TC_REG = (1U << pin);
}
