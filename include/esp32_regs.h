#ifndef ESP32_REGS_H
#define ESP32_REGS_H

#include <stdint.h>

/* GPIO Matrix registers */
#define GPIO_ENABLE_REG     (*(volatile uint32_t *)0x3FF44020)
#define GPIO_ENABLE_W1TS    (*(volatile uint32_t *)0x3FF44024)
#define GPIO_OUT_REG        (*(volatile uint32_t *)0x3FF44004)
#define GPIO_OUT_W1TS_REG   (*(volatile uint32_t *)0x3FF44008)
#define GPIO_OUT_W1TC_REG   (*(volatile uint32_t *)0x3FF4400C)
#define GPIO_IN_REG         (*(volatile uint32_t *)0x3FF4403C)

/* UART0 registers */
#define UART0_BASE          0x3FF40000
#define UART_FIFO_REG       (*(volatile uint32_t *)(UART0_BASE + 0x00))
#define UART_STATUS_REG     (*(volatile uint32_t *)(UART0_BASE + 0x1C))
#define UART_CLKDIV_REG     (*(volatile uint32_t *)(UART0_BASE + 0x14))
#define UART_CONF0_REG      (*(volatile uint32_t *)(UART0_BASE + 0x20))

#define GPIO2_BIT           (1U << 2)

#endif /* ESP32_REGS_H */