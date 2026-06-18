#include "esp32_regs.h"

void gpio_init_output(int pin) {
    // GPIO2 IO MUX register is at 0x3FF49040
    // Function select (MCU_SEL) is a 3-bit field at bits 12-14, not 4-5.
    // PIN_FUNC_GPIO = 2 selects the plain GPIO function on this pad.
    volatile uint32_t *iomux = (volatile uint32_t *)0x3FF49040;
    *iomux = (2 << 12);  // MCU_SEL = 2 (GPIO function)

    // GPIO_FUNCx_OUT_SEL_CFG_REG is intentionally left untouched: every
    // pin's matrix output-select already resets to 0x100 (SIG_GPIO_OUT_IDX,
    // i.e. "driven directly by GPIO_OUT_REG") at boot, so no write is
    // needed for a fresh pin. The previous version wrote to 0x3FF44554,
    // which is GPIO_FUNC9_OUT_SEL_CFG_REG (pin 9, not pin 2) -- GPIO9 is
    // wired internally to the module's SPI flash chip, so that write
    // broke flash access and crashed the chip almost immediately.

    GPIO_ENABLE_W1TS = (1U << pin);
}

void gpio_set(int pin) {
    GPIO_OUT_W1TS_REG = (1U << pin);
}

void gpio_clear(int pin) {
    GPIO_OUT_W1TC_REG = (1U << pin);
}