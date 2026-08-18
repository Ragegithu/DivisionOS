BUILD_DIR?=Build/
CC=wcc386
CFLAGS=-3 -bt=dos4g -mf -wx -zl -s -za99

.PHONY: all kernel clean

all: kernel

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: kernel.c stdio.c idt.c shell.c keyboard.c kernel.asm
	nasm -f obj kernel.asm -o $(BUILD_DIR)/kernel_asm.obj
	$(CC) $(CFLAGS) kernel.c -fo=$(BUILD_DIR)/kernel.obj
	$(CC) $(CFLAGS) stdio.c -fo=$(BUILD_DIR)/stdio.obj
	$(CC) $(CFLAGS) idt.c -fo=$(BUILD_DIR)/idt.obj
	$(CC) $(CFLAGS) keyboard.c -fo=$(BUILD_DIR)/keyboard.obj
	$(CC) $(CFLAGS) shell.c -fo=$(BUILD_DIR)/shell.obj
	wlink FILE $(BUILD_DIR)/kernel_asm.obj,$(BUILD_DIR)/kernel.obj,$(BUILD_DIR)/stdio.obj,$(BUILD_DIR)/idt.obj,$(BUILD_DIR)/keyboard.obj,$(BUILD_DIR)/shell.obj NAME $(BUILD_DIR)/kernel.bin @kernel.lnk

clean:
	rm -rf $(BUILD_DIR)/kernel.bin
	rm -rf $(BUILD_DIR)/kernel.obj
	rm -rf $(BUILD_DIR)/stdio.obj
	rm -rf $(BUILD_DIR)/idt.obj
	rm -rf $(BUILD_DIR)/keyboard.obj
	rm -rf $(BUILD_DIR)/kernel_asm.obj
	rm -rf $(BUILD_DIR)/shell.obj