# Copy your IWAD to the IWAD_DIRECTORY folder and select corresponding prefix here
IWAD_DIRECTORY = iwads
# Options: DOOM1 DOOMR DOOM DOOM2 PLUTONIA TNT
IWAD_PREFIX = DOOM

SOURCE_DIR = src
BUILD_DIR = build

include $(N64_INST)/include/n64.mk

N64_ROM_TITLE = $(IWAD_PREFIX)

# Uncomment to compile with various safety checks throughout the code
# CFLAGS += -DRANGECHECK

all: $(IWAD_PREFIX).z64

ASSETS_MENULUMPS = $(wildcard assets/menulumps/*.bin)
ASSETS_MIDI_INSTRUMENTS = assets/MIDI_Instruments.bin
ASSETS_IWAD = $(IWAD_DIRECTORY)/$(IWAD_PREFIX).WAD

FS_MENULUMPS = $(addprefix filesystem/menulumps/, $(notdir $(ASSETS_MENULUMPS:%.bin=%.bin)))
FS_MIDI_INSTRUMENTS = $(subst assets/, filesystem/, $(ASSETS_MIDI_INSTRUMENTS))
FS_IWAD = $(subst $(IWAD_DIRECTORY)/, filesystem/, $(ASSETS_IWAD))
FS_IWAD_IDENTIFIER = filesystem/iwad_identifier.txt

filesystem/menulumps/%.bin: assets/menulumps/%.bin
	@mkdir -p $(dir $@)
	@echo "    [ASSET] $@"
	@cp "$<" "$(dir $@)"

$(FS_MIDI_INSTRUMENTS): $(ASSETS_MIDI_INSTRUMENTS)
	@mkdir -p $(dir $@)
	@echo "    [ASSET] $@"
	@cp "$<" "$(dir $@)"

$(FS_IWAD): $(ASSETS_IWAD)
	@mkdir -p $(dir $@)
	@echo "    [ASSET] $@"
	@cp "$<" "$(dir $@)"

$(FS_IWAD_IDENTIFIER):
	@echo $(IWAD_PREFIX).WAD > $(FS_IWAD_IDENTIFIER)

filesystem/: $(FS_MENULUMPS) $(FS_MIDI_INSTRUMENTS) $(FS_IWAD) $(FS_IWAD_IDENTIFIER)

$(BUILD_DIR)/$(IWAD_PREFIX).dfs: filesystem/ $(FS_MENULUMPS) $(FS_MIDI_INSTRUMENTS) $(FS_IWAD) $(FS_IWAD_IDENTIFIER)

SOURCE_FILES := $(shell find $(SOURCE_DIR)/ -type f -name '*.c' | sort)
OBJECT_FILES := $(SOURCE_FILES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/$(IWAD_PREFIX).elf: $(OBJECT_FILES)

$(IWAD_PREFIX).z64: $(BUILD_DIR)/$(IWAD_PREFIX).dfs

clean:
	rm -rf $(BUILD_DIR) filesystem/ *.z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean
