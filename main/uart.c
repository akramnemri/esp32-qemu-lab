#include "esp32_regs.h"

void uart_init(void) {
    UART_CLKDIV_REG = (80000000 / 115200) << 4;
    UART_CONF0_REG = 0x00000003;
}

void uart_putc(char c) {
    while (UART_STATUS_REG & 0x00400000);
    UART_FIFO_REG = c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}
