# DivisionOS
Custom made operating system with custom 2 stage bootloader.

# DivisionOS

A bare-metal x86 operating system built from scratch in Assembly and C,  
targeting the i486 processor. Runs on QEMU. Built out of curiosity  
and love for how computers actually work.

## What it does

- **Stage 1 Bootloader** — fits in the 512-byte MBR, initializes the  
  environment and loads Stage 2
- **Stage 2 Bootloader** — sets up the environment for freestanding C  
  and loads the kernel
- **Kernel** — boots into a minimal shell with a working keyboard driver

## Shell Commands

| Command | Description |
|---------|-------------|
| `help`  | "there aint no help for you round here" |
| `clear` | Clears the screen |
| `name`  | Reprints the DivisionOS banner |

## Tech

- **Architecture:** x86 (i486)
- **Language split:** ~70% Assembly, ~30% C (freestanding)
- **Emulator:** QEMU

## Running it

```bash
qemu-system-i386 -cpu 486 -fda divisionos.img
```

## Why

Most people never touch the code that runs before the OS.  
I wanted to understand what actually happens when a machine boots —  
from the first instruction the BIOS hands control to, all the way to  
a C environment I built myself.
