#include "esp32_regs.h"

void uart_init(void) {
    // Bootloader already configured UART0 at 115200 baud.
    // No reconfiguration needed for bare-metal output.
}

void uart_putc(char c) {
    while (UART_STATUS_REG & 0x00400000);  // Wait while TX FIFO full
    UART_FIFO_REG = c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}