# ESP32 QEMU Bare-Metal Lab

## Overview

This project demonstrates register-level programming on the ESP32 using direct memory-mapped peripheral access rather than high-level frameworks such as Arduino.

The application is developed and built using ESP-IDF, executed inside QEMU, and validated through UART output. GPIO and UART peripherals are controlled directly through hardware registers, providing a closer look at embedded systems development and ESP32 architecture.

The project was developed inside WSL2 and uses the Xtensa toolchain supplied by ESP-IDF.

---

## Features

* Direct GPIO register manipulation
* Direct UART register manipulation
* Bare-metal style programming
* ESP-IDF build system integration
* QEMU-based execution
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
    └── QEMU
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

## Creating the Flash Image

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
```

Resize image to match flash size:

```bash
truncate -s 4M build/qemu_flash.bin
```

---

## Running in QEMU

Launch the emulator:

```bash
qemu-system-xtensa -nographic \
    -machine esp32 \
    -drive file=build/qemu_flash.bin,if=mtd,format=raw \
    -serial mon:stdio \
    -global driver=timer.esp32.timg,property=wdt_disable,value=true
```

---

## Expected Output

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

---

## Validation

The project was validated through:

* Successful ESP-IDF compilation
* Successful flash image generation
* Successful ESP32 boot within QEMU
* UART output verification
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

Solution:

Configured ESP-IDF exports and added permanent environment initialization through `.bashrc`.

### Invalid Flash Image Layout

QEMU rejected early flash images due to incorrect binary placement.

Solution:

Used `esptool.py merge_bin` with the proper ESP32 flash offsets.

### Bare-Metal Linker Issues

Custom startup and linker configurations caused build instability.

Solution:

Migrated to an ESP-IDF Minimal Project architecture while preserving direct register-level programming.

### QEMU Serial Conflict

QEMU monitor and UART attempted to share standard input/output.

Solution:

Used:

```bash
-serial mon:stdio
```

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
