# Makefile for RISC-V Firmware

# Toolchain
CC = /opt/riscv/bin/riscv64-unknown-elf-gcc
AS = /opt/riscv/bin/riscv64-unknown-elf-as
LD = /opt/riscv/bin/riscv64-unknown-elf-ld
OBJCOPY = /opt/riscv/bin/riscv64-unknown-elf-objcopy
OBJDUMP = /opt/riscv/bin/riscv64-unknown-elf-objdump

# Flags
CFLAGS = -march=rv64gc -mabi=lp64 -mcmodel=medany -O2 -g -Wall -nostdlib -ffreestanding -nostartfiles
ASFLAGS = -march=rv64gc -mabi=lp64 -g
LDFLAGS = -nostdlib -T link.ld

# Source files
C_SRCS = main.c firmware.c supervisor.c user_apps.c
ASM_SRCS = firmware_asm.S supervisor_asm.S crt0.S
OBJS = $(C_SRCS:.c=.o) $(ASM_SRCS:.S=.o)

# Target
TARGET = firmware

all: $(TARGET).elf $(TARGET).bin $(TARGET).dump

$(TARGET).elf: $(OBJS) link.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).dump: $(TARGET).elf
	$(OBJDUMP) -D $< > $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) -c $< -o $@

drun: all
	/opt/riscv/bin/spike -l --log-commits firmware.elf

run: all
	/opt/riscv/bin/spike firmware.elf

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin $(TARGET).dump

.PHONY: all run clean
