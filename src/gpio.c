#include "esp32_regs.h"

void gpio_init_output(int pin) {
    // GPIO2 IO MUX register is at 0x3FF49040
    // Function select (MCU_SEL) is a 3-bit field at bits 12-14.
    // PIN_FUNC_GPIO = 2 selects the plain GPIO function on this pad.
    volatile uint32_t *iomux = (volatile uint32_t *)0x3FF49040;
    *iomux = (2 << 12);  // MCU_SEL = 2 (GPIO function)

    // GPIO_FUNCx_OUT_SEL_CFG_REG is intentionally left untouched:
    // the pin's matrix output-select already resets to 0x100 at boot,
    // so no write is needed for a fresh pin.
    GPIO_ENABLE_W1TS = (1U << pin);
}

void gpio_set(int pin) {
    GPIO_OUT_W1TS_REG = (1U << pin);
}

void gpio_clear(int pin) {
    GPIO_OUT_W1TC_REG = (1U << pin);
}