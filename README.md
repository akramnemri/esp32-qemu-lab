# ESP32 QEMU Bare-Metal Lab

A compact project for building and running an ESP32 firmware image with ESP-IDF, testing it in QEMU, and flashing it to a physical ESP32 board.

## Overview

This project demonstrates register-level ESP32 control for GPIO and UART while using ESP-IDF for the build system and runtime support. It can be tested in QEMU or on real hardware.

## Key Features

- ESP-IDF-based build flow
- QEMU execution for ESP32 firmware
- Physical ESP32 flashing support
- GPIO2 LED blink pattern
- UART output for runtime verification

## Project Structure

```text
esp32-qemu-lab/
├── CMakeLists.txt
├── README.md
│
├── src/ # code C (implementation .c)
│ ├── main.c
│ ├── gpio.c
│ ├── uart.c
│
├── include/ # headers (.h)
│ ├── esp32_regs.h
│
├── qemu_lab/
├── arduino_flashing_blink_led.ino
├── docs/
```

## Requirements

- Ubuntu, WSL2 Ubuntu, or another Debian-based Linux environment
- ESP-IDF installed for ESP32
- Xtensa ESP32 toolchain
- Python 3
- esptool.py
- QEMU with ESP32 support

## Setup

Clone the repository:

```bash
git clone https://github.com/akramnemri/esp32-qemu-lab.git
cd esp32-qemu-lab
```

Export the ESP-IDF environment:

```bash
source ~/esp/esp-idf/export.sh
```

Verify the tools:

```bash
idf.py --version
xtensa-esp-elf-gcc --version
qemu-system-xtensa --version
```

If `qemu-system-xtensa` is missing, install the ESP-IDF QEMU tools or the recommended QEMU package for your environment, then re-export the ESP-IDF environment.

## Build

```bash
idf.py set-target esp32
idf.py build
```

A successful build generates:

```text
build/bootloader/bootloader.bin
build/partition_table/partition-table.bin
build/main.bin
```

## Run in QEMU

Create the flash image:

```bash
esptool.py --chip esp32 merge_bin   -o build/qemu_flash.bin   --flash_mode dio   --flash_size 4MB   --flash_freq 40m   0x1000 build/bootloader/bootloader.bin   0x8000 build/partition_table/partition-table.bin   0x10000 build/main.bin

truncate -s 4M build/qemu_flash.bin
```

Launch QEMU:

```bash
qemu-system-xtensa -nographic   -machine esp32   -drive file=build/qemu_flash.bin,if=mtd,format=raw   -serial mon:stdio   -global driver=timer.esp32.timg,property=wdt_disable,value=true
```

Stop QEMU with `Ctrl+A` then `X`.

## Flash to Physical ESP32

For real hardware, use an ESP32 board with GPIO2 connected to the onboard LED.

Build and flash:

```bash
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

If you are using WSL2 for physical flashing, attach the USB device to WSL2 first with `usbipd`.

## Expected Output

In QEMU and on hardware, the program prints a repeating UART message sequence and blinks the GPIO2 LED with the project pattern.

## Troubleshooting

- `xtensa-esp-elf-gcc` or `idf.py` not found: re-run `source ~/esp/esp-idf/export.sh`
- `qemu-system-xtensa: unsupported machine type`: make sure you are using the ESP32-enabled QEMU from ESP-IDF tools, not only the generic Ubuntu package
- `Could not open build/qemu_flash.bin`: run QEMU from the project root
- No output in QEMU: confirm the flash image was created with `merge_bin` and padded to 4 MB
- No serial device in WSL2: attach the board with `usbipd` and confirm `/dev/ttyUSB0`

## Author

Akram Nemri
