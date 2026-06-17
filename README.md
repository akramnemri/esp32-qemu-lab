# ESP32 QEMU Bare-Metal Lab

## Overview

This project demonstrates register-level programming on the ESP32 using direct memory-mapped peripheral access rather than high-level frameworks such as Arduino.

The application is developed and built using ESP-IDF, executed inside QEMU or flashed to physical hardware, and validated through UART output. GPIO and UART peripherals are controlled directly through hardware registers, providing a closer look at embedded systems development and ESP32 architecture.

The project was developed inside WSL2 and uses the Xtensa toolchain supplied by ESP-IDF.

---

## Features

* Direct GPIO register manipulation
* Direct UART register manipulation
* Bare-metal style programming
* ESP-IDF build system integration
* QEMU-based execution
* Physical ESP32 hardware flashing
* Flash image generation using esptool.py
* UART-based runtime validation

---

## Project Architecture

```text
Windows
│
└── WSL2 (Ubuntu)
    │
    ├── ESP-IDF
    ├── Xtensa Toolchain
    ├── esptool.py
    ├── QEMU
    └── Physical ESP32 (optional)
            │
            └── ESP32 Firmware
                    │
                    ├── UART Driver
                    ├── GPIO Driver
                    └── Application Logic
```

---

## Directory Structure

```text
esp32-qemu-lab/
│
├── main/
│   ├── main.c
│   ├── gpio.c
│   ├── uart.c
│   └── esp32_regs.h
│
├── build/
│
├── CMakeLists.txt
├── sdkconfig
└── README.md
```

---

## Hardware Concept

The application controls:

* UART0 for serial communication
* GPIO2 as an output pin

A repeating pattern is executed:

```text
2 Quick Blinks
↓
2 Long Blinks
↓
2 Second Hold
↓
Repeat
```

UART messages are printed during execution to verify runtime behavior.

---

## Prerequisites

### Operating System

* Windows 10/11
* WSL2 with Ubuntu

### Required Software

* ESP-IDF
* Xtensa ESP32 Toolchain
* Python
* esptool.py
* QEMU with ESP32 support

---

## Tools and Dependencies

This project relies on several open-source tools provided by the embedded systems community. The project itself focuses on implementing register-level ESP32 programming, while these tools provide the build system, compiler, flashing utilities, and emulation environment.

| Tool                   | Purpose                                                                        | Author / Maintainer       |
| ---------------------- | ------------------------------------------------------------------------------ | ------------------------- |
| ESP-IDF                | Official ESP32 development framework used for building and linking the project | Espressif Systems         |
| Xtensa ESP32 Toolchain | Cross-compiler used to generate ESP32 binaries                                 | Espressif Systems         |
| esptool.py             | Utility used to create and manipulate ESP32 flash images                       | Espressif Systems         |
| QEMU                   | Hardware emulator used to execute the firmware without physical hardware       | QEMU Project Contributors |
| WSL2                   | Linux environment used for development on Windows                              | Microsoft                 |

### ESP-IDF

Official development framework for ESP32 devices.

Repository:

https://github.com/espressif/esp-idf

Author:

Espressif Systems

Used in this project for:

* Build system integration
* Startup code initialization
* Linker configuration
* Memory layout management
* Toolchain integration

### Xtensa ESP32 Toolchain

Cross-compilation toolchain used to generate firmware for the ESP32 Xtensa LX6 architecture.

Repository:

https://github.com/espressif/crosstool-NG

Author:

Espressif Systems

### esptool.py

Official ESP32 image and flashing utility.

Repository:

https://github.com/espressif/esptool

Author:

Espressif Systems

Used in this project to:

* Merge binaries
* Create flash images
* Validate image layout
* Flash physical hardware

### QEMU

Open-source machine emulator and virtualizer.

Website:

https://www.qemu.org/

Repository:

https://gitlab.com/qemu-project/qemu

Authors:

QEMU Project Contributors

Used in this project to:

* Emulate ESP32 hardware
* Execute firmware
* Display UART output
* Validate application behavior

### WSL2

Windows Subsystem for Linux version 2.

Website:

https://learn.microsoft.com/windows/wsl/

Author:

Microsoft

Used in this project as the primary development environment.

---

## Environment Setup

### Create Project Directory

```bash
mkdir -p ~/esp32-qemu-lab
cd ~/esp32-qemu-lab
```

### Create Working Structure

```bash
mkdir -p src include build
```

### Export ESP-IDF Environment

```bash
source ~/esp/esp-idf/export.sh
```

### Verify Installation

```bash
idf.py --version
xtensa-esp-elf-gcc --version
qemu-system-xtensa --version
```

---

## Permanent PATH Configuration

To avoid exporting the environment manually every session, add the following to your `.bashrc`:

```bash
echo 'source ~/esp/esp-idf/export.sh' >> ~/.bashrc
source ~/.bashrc
```

Verify:

```bash
which idf.py
which xtensa-esp-elf-gcc
which qemu-system-xtensa
```

---

## ESP-IDF Minimal Project Approach

Originally, the project was attempted as a fully bare-metal application with a custom startup sequence and linker configuration.

However, linker and runtime initialization issues made this approach unnecessarily complex.

The final solution uses an ESP-IDF Minimal Project architecture.

ESP-IDF provides:

* Startup code
* Linker scripts
* Memory layout
* Build infrastructure

The application code remains register-level and directly manipulates ESP32 peripherals without using ESP-IDF drivers.

This hybrid approach preserves low-level control while ensuring reliable execution.

---

## Source Code Components

### uart.c

Provides UART initialization and serial output functions.

Responsibilities:

* UART configuration
* Character transmission
* String transmission

### gpio.c

Provides GPIO register-level access.

Responsibilities:

* GPIO configuration
* Set output high
* Set output low

### esp32_regs.h

Contains memory-mapped register definitions for:

* UART0
* GPIO subsystem

### main.c

Implements application behavior:

* Delay routines
* Blink patterns
* Runtime UART messages

---

## Final Source Code

### main/esp32_regs.h

```c
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
```

### main/gpio.c

```c
#include "esp32_regs.h"

void gpio_init_output(int pin) {
    /* GPIO2 IO MUX register is at 0x3FF49040 */
    /* MCU_SEL (function select) is bits 12-14, value 2 = GPIO function */
    volatile uint32_t *iomux = (volatile uint32_t *)0x3FF49040;
    *iomux = (2 << 12);  /* PIN_FUNC_GPIO = 2 */

    /* GPIO Matrix: route GPIO peripheral output to pin 2 */
    /* GPIO_FUNCn_OUT_SEL_CFG_REG base: 0x3FF44530, 4 bytes per pin */
    /* GPIO2 register: 0x3FF44530 + 2*4 = 0x3FF44538 */
    /* Value 0x100 (256) = select GPIO_OUT_REG[2] as source */
    volatile uint32_t *func_out_sel = (volatile uint32_t *)0x3FF44538;
    *func_out_sel = 0x100;

    GPIO_ENABLE_W1TS = (1U << pin);
}

void gpio_set(int pin) {
    GPIO_OUT_W1TS_REG = (1U << pin);
}

void gpio_clear(int pin) {
    GPIO_OUT_W1TC_REG = (1U << pin);
}
```

### main/uart.c

```c
#include "esp32_regs.h"

void uart_init(void) {
    /* Bootloader already configured UART0 at 115200 baud. */
    /* No reconfiguration needed for bare-metal output. */
}

void uart_putc(char c) {
    while (UART_STATUS_REG & 0x00400000);  /* Wait while TX FIFO full */
    UART_FIFO_REG = c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}
```

### main/main.c

```c
#include <stdint.h>
#include "esp32_regs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void gpio_init_output(int pin);
void gpio_set(int pin);
void gpio_clear(int pin);
void uart_init(void);
void uart_puts(const char *s);

#define QUICK_MS    200
#define LONG_MS     800
#define HOLD_MS     2000

void delay_ms(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
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

    uart_puts("
=== ESP32 QEMU Bare-Metal ===
");
    uart_puts("Pattern: 2 quick -> 2 long -> 2s on -> 2s off
");
    uart_puts("================================

");

    while (1) {
        uart_puts("CYCLE START
");

        blink_quick();
        blink_quick();
        uart_puts("  2 quick done
");

        blink_long();
        blink_long();
        uart_puts("  2 long done
");

        hold_steady();
        uart_puts("  hold done

");
    }
}
```

### Root CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp32_qemu_lab)
```

### main/CMakeLists.txt

```cmake
idf_component_register(SRCS "main.c" "gpio.c" "uart.c"
                    INCLUDE_DIRS ".")
```

---

## Building the Project

Build using ESP-IDF:

```bash
idf.py build
```

Generated artifacts include:

```text
bootloader.bin
partition-table.bin
main.bin
```

---

## Running in QEMU

### Create the QEMU Flash Image

Merge the generated binaries into a single ESP32 flash image:

```bash
cd ~/esp32-qemu-lab

esptool.py --chip esp32 merge_bin \
    -o build/qemu_flash.bin \
    --flash_mode dio \
    --flash_size 4MB \
    --flash_freq 40m \
    0x1000 build/bootloader/bootloader.bin \
    0x8000 build/partition_table/partition-table.bin \
    0x10000 build/main.bin

truncate -s 4M build/qemu_flash.bin
```

### Launch QEMU

```bash
qemu-system-xtensa -nographic \
    -machine esp32 \
    -drive file=build/qemu_flash.bin,if=mtd,format=raw \
    -serial mon:stdio \
    -global driver=timer.esp32.timg,property=wdt_disable,value=true
```

To stop QEMU, press **Ctrl+A** then **X**. If you need the monitor, press **Ctrl+A** then **C** and type `quit`.

---

## Flashing to Physical ESP32 Hardware

### Prerequisites

* Physical ESP32 dev board (e.g., ESP32-DevKitC, ESP32-WROOM-32)
* USB cable (data-capable, not charge-only)
* USB-to-UART bridge drivers installed on host (CP210x, CH340, or FTDI)

### WSL2 USB Setup

WSL2 does not natively expose USB serial devices. Install `usbipd` on Windows to attach the ESP32 to WSL2.

**Install usbipd on Windows (PowerShell as Administrator):**

```powershell
winget install --interactive --exact dorssel.usbipd-win
```

**Attach ESP32 to WSL2:**

```powershell
usbipd list
usbipd bind --busid <BUSID>
usbipd attach --wsl --busid <BUSID>
```

**Inside WSL2, grant port permissions:**

```bash
sudo chmod 666 /dev/ttyUSB0
```

**Permanent fix (add user to dialout group):**

```bash
sudo usermod -a -G dialout $USER
```

Log out and back in for the group change to take effect.

### Build and Flash

```bash
cd ~/esp32-qemu-lab
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

To re-flash, put the board into download mode:
1. Hold **BOOT** button
2. Press and release **EN** (RESET)
3. Release **BOOT**

### Exit Monitor

Press **Ctrl+]** to exit `idf.py monitor`.

---

## Expected Output

### QEMU Output

```text
=== ESP32 QEMU Bare-Metal ===
Pattern: 2 quick -> 2 long -> 2s on -> 2s off
================================

CYCLE START
  2 quick done
  2 long done
  hold done
```

The output should continuously repeat.

### Physical Hardware Output

The onboard LED on GPIO2 will blink: **2 quick → 2 long → 2s on → 2s off → repeat**. UART output at 115200 baud shows the same messages as QEMU.

---

## Validation

The project was validated through:

* Successful ESP-IDF compilation
* Successful flash image generation
* Successful ESP32 boot within QEMU
* UART output verification in QEMU
* Successful flashing to physical ESP32-D0WD-V3
* UART output verification on physical hardware
* LED blink pattern verification on physical hardware
* Continuous execution of the programmed pattern

Validation Commands:

```bash
idf.py --version
xtensa-esp-elf-gcc --version
qemu-system-xtensa --version
```

---

## Challenges Encountered

### Toolchain Configuration

Initial environment configuration issues prevented the compiler from being detected.

**Solution:** Configured ESP-IDF exports and added permanent environment initialization through `.bashrc`.

### Invalid Flash Image Layout

QEMU rejected early flash images due to incorrect binary placement.

**Solution:** Used `esptool.py merge_bin` with the proper ESP32 flash offsets.

### Bare-Metal Linker Issues

Custom startup and linker configurations caused build instability.

**Solution:** Migrated to an ESP-IDF Minimal Project architecture while preserving direct register-level programming.

### QEMU Serial Conflict

QEMU monitor and UART attempted to share standard input/output.

**Solution:** Used `-serial mon:stdio`.

### Watchdog Timer Resets on Physical Hardware

The ESP32's Task Watchdog and Interrupt Watchdog triggered resets during the blocking delay loop.

**Solution:** Disabled watchdogs in `sdkconfig`:

```bash
# In sdkconfig
# CONFIG_ESP_INT_WDT is not set
# CONFIG_ESP_TASK_WDT_EN is not set
```

### GPIO Register Address Bug on Physical Hardware

A wrong GPIO Matrix register address (`0x3FF44554` instead of `0x3FF44538`) caused the firmware to write to `GPIO_FUNC9_OUT_SEL_CFG_REG`, which is connected to the on-board SPI flash chip. This severed the flash connection and crashed the chip immediately after `app_main()` started, producing no UART output and no LED blink.

**Root Cause:** QEMU does not simulate the physical GPIO9-to-flash wiring, so the same bug was harmless in emulation but fatal on silicon.

**Solution:** Corrected the GPIO Matrix base address to `0x3FF44530` and calculated the pin offset as `base + pin * 4`. Also fixed the IO MUX `MCU_SEL` field to bits 12-14 with value `2` for GPIO function (`PIN_FUNC_GPIO`).

### UART Baud Rate Configuration

The UART clock divisor register format differs between QEMU's forgiving emulation and real hardware.

**Solution:** The bootloader already configures UART0 correctly at 115200 baud. Bare-metal UART output works without reinitialization by using the bootloader's existing configuration.

---

## Future Improvements

* Interrupt-driven UART communication
* Hardware timer integration
* FreeRTOS task implementation
* Additional ESP32 peripheral support
* Automated build and test scripts
* GitHub Actions CI/CD pipeline

---

## Acknowledgements

This project relies on the work of the open-source communities behind ESP-IDF, esptool.py, QEMU, WSL2, and the supporting development tools maintained by Espressif Systems, Microsoft, and the broader embedded systems community.

---

## Author

**Akram Nemri**

Computer Science Student

Embedded Systems • Low-Level Programming • Systems Development
