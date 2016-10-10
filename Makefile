###############################################################################
# ARM Assembly/C cross-compiler makefile
###############################################################################

# The toolchain to use. Can be overridden by previous definition
ARMGNU ?= arm-linux-gnueabihf

# Directory for compiled object files.
BUILD = build

# Base source directory
SOURCE = source

TARGET = $(BUILD)/rash

# The name of the assembler listing file to generate.
LIST = $(TARGET).list

# The name of the binary (.bin) file to generate
BIN = $(TARGET).bin

# The name of the hex (.hex) file to generate
HEX = $(TARGET).hex

# The name of the map file to generate.
MAP = $(TARGET).map


# -O0 -- turn off all C optimization
CFLAGS = -Wall -Werror -O0 -g -std=c11
LDFLAGS = -std=c11 -Wl,--gc-sections,--print-gc-sections


OBJECTS := $(SOURCE)/rshell.c

# Rule to make everything.
all: $(BUILD) $(TARGET)

# Rule to remake everything. Does not include clean.
rebuild: all

$(TARGET) : $(OBJECTS)
	$(ARMGNU)-gcc $(LDFLAGS) -o $(TARGET) $(OBJECTS)


# Rule to make the object files.
$(BUILD)/%.o: $(SOURCE)/%.s $(BUILD)
	$(ARMGNU)-as -I $(SOURCE) $< -o $@

$(BUILD)/%.o: $(SOURCE)/%.c $(BUILD)
	$(ARMGNU)-gcc -I $(SOURCE) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir $@

# Rule to clean files.
clean :
	-rm -rf $(BUILD)
