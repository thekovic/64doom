# Copy your IWAD to the IWAD_DIRECTORY folder and select corresponding prefix here
IWAD_DIRECTORY = iwads
# Options: doom1 doomr doom doom2 plutonia tnt
IWAD_PREFIX = doom
# PWAD related shinanigans
# This requires deutex until we have a better system.
# Copy it in the root or leave it be if you installed it as a package ie used the AUR
PWAD_DIRECTORY = pwads
PWAD_PREFIX = sigil

SOURCE_DIR = src
BUILD_DIR = build

include $(N64_INST)/include/n64.mk

N64_ROM_TITLE = $(IWAD_PREFIX)

# Uncomment to compile with various safety checks throughout the code
# CFLAGS += -DRANGECHECK

# Uncomment to compile without visible FPS counter
# CFLAGS += -DNO_FPS_COUNTER

all: $(IWAD_PREFIX).z64

ASSETS_MENULUMPS = $(wildcard assets/menulumps/*.bin)
ASSETS_MIDI_INSTRUMENTS = assets/MIDI_Instruments.bin
ASSETS_IWAD = $(IWAD_DIRECTORY)/$(IWAD_PREFIX).wad
ASSETS_PWAD = $(PWAD_DIRECTORY)/$(PWAD_PREFIX).wad

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

pwad:
# This is convoluted, deutex can't do file paths at least on UNIX
# That suck because libdragon is easiest set up even on Windows through WSL
# We are getting aound this by juggling files around
# This should also help prevent meeting meet an Eric Harris impersonator over deleted IWADS
	@echo "merging $(PWAD_PREFIX).wad into $(IWAD_PREFIX).wad"
	@cp $(ASSETS_IWAD) $(IWAD_PREFIX).wad
	deutex -merge $(ASSETS_PWAD)
# Chances are this path doesnt exist yet, let me do that for you
	@mkdir -p filesystem/
	@mv "$(IWAD_PREFIX).wad" filesystem/$(IWAD_PREFIX).wad
	@echo "You can now use gnu make to enjoy $(PWAD_PREFIX). Rip and tear!!!"

clean:
	rm -rf $(BUILD_DIR) filesystem/ $(IWAD_PREFIX).z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean pwad
