# DivisionOS

A bare-metal x86 operating system built from scratch in Assembly and C,
targeting the i486 processor. Runs on QEMU. Built out of curiosity
and love for how computers actually work.

## What it does

- **Stage 1 Bootloader** — fits in the 512-byte MBR, implements a FAT12
  filesystem driver to locate and load Stage 2 from disk
- **Stage 2 Bootloader** — implements a FAT12 driver to locate and load
  the kernel, sets up the environment for freestanding C
- **Kernel** — boots into a minimal shell with a working keyboard driver
- **FAT Tool** — host-side utility for inspecting the floppy image

## Shell Commands

| Command | Description |
|---------|-------------|
| `help`  | "there aint no help for you round here" |
| `clear` | Clears the screen |
| `name`  | Reprints the DivisionOS banner |

## Tech

- **Architecture:** x86 (i486)
- **Language split:** ~63% C, ~29% Assembly, ~8% Makefile
- **Filesystem:** FAT12 — implemented from scratch in both bootloader stages
- **Assembler:** NASM
- **Compiler:** GCC (kernel) + Watcom (16-bit C for bootloader stages)
- **Emulator:** QEMU

## Dependencies

- `nasm`
- `gcc`
- `mtools` (mcopy, mkfs.fat)
- Watcom compiler for 16-bit C — expected at `/home/rage/watcom/binl64/`

## Building & Running

```bash
# build everything (bootloader, kernel, floppy image)
make

# run in QEMU
qemu-system-i386 -cpu 486 -fda Build/main_floppy.img

# clean
make clean
```

## Why

Most people never touch the code that runs before the OS.
I wanted to understand what actually happens when a machine boots —
from the first instruction the BIOS hands control to, all the way to
a FAT12 filesystem driver and freestanding C environment I built myself.

> The OS was originally named NBOS (from the amazing nanobyte) — you can still spot it as the FAT12  
> volume label in the Makefile.
