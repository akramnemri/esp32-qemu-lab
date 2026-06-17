#include <stdint.h>
#include "esp32_regs.h"

void gpio_init_output(int pin);
void gpio_set(int pin);
void gpio_clear(int pin);
void uart_init(void);
void uart_puts(const char *s);

#define QUICK_MS    200
#define LONG_MS     800
#define HOLD_MS     2000

void delay_ms(uint32_t ms) {
    uint32_t cycles = ms * 240000;
    while (cycles--) {
        __asm__ volatile ("nop");
    }
}

void blink_quick(void) {
    gpio_set(2);
    delay_ms(QUICK_MS);
    gpio_clear(2);
    delay_ms(QUICK_MS);
}

void blink_long(void) {
    gpio_set(2);
    delay_ms(LONG_MS);
    gpio_clear(2);
    delay_ms(LONG_MS);
}

void hold_steady(void) {
    gpio_set(2);
    delay_ms(HOLD_MS);
    gpio_clear(2);
    delay_ms(HOLD_MS);
}

void app_main(void) {
    uart_init();
    gpio_init_output(2);
    
    uart_puts("\r\n=== ESP32 QEMU Bare-Metal ===\r\n");
    uart_puts("Pattern: 2 quick -> 2 long -> 2s on -> 2s off\r\n");
    uart_puts("================================\r\n\r\n");
    
    while (1) {
        uart_puts("CYCLE START\r\n");
        
        blink_quick();
        blink_quick();
        uart_puts("  2 quick done\r\n");
        
        blink_long();
        blink_long();
        uart_puts("  2 long done\r\n");
        
        hold_steady();
        uart_puts("  hold done\r\n\r\n");
    }
}
