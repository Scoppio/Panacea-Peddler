# build a program from 2 files and one shared header

# OBJECT dependencies for the game to be correctly compiled
_DEPS = main.o crt0.o

# Compile C to asm
CC = cc65
# Compile Asm to Object file
CA = ca65
# Link object files in a single final file
LD = ld65

CC65FLAGS = --add-source --all-cdecl --eagerly-inline-funcs --inline-stdfuncs --check-stack -t nes --standard cc65
CFLAGS = -g --cpu 6502X
LDFLAGS = -C nrom_32k_vert.cfg
BUILD_FILES = nes.lib -Ln

ODIR=output
_LABELS = game.labels.txt
EXTRAS = -m $(patsubst %,$(ODIR)/%, game.map.txt) --dbgfile $(patsubst %,$(ODIR)/%, game.nes.dbg)

_ASSETS_MAPS = assets
ASSETS_MAPS = $(patsubst %, $(_ASSETS_MAPS)/%, maps)

DEPS = $(patsubst %,$(ODIR)/%,$(_DEPS))
LABELS = $(patsubst %,$(ODIR)/%,$(_LABELS))

MKDIR_P = mkdir 

ifeq (${OS},Windows_NT)
		REMOVE = del /Q
else
        REMOVE = rm
endif

# process that creates the output folder if not present

# Build process - c to asm files
$(ODIR)/%.s: %.c
	$(CC) -o $@ -Oirs $^ $(CC65FLAGS)

# Generated Asm file to object file
$(ODIR)/%.o: %.s
	$(CA) -o $@ $^ $(CFLAGS)

# Handcrafted Asm file to object file
$(ODIR)/%.o: $(ODIR)/%.s
	$(CA) -o $@ $^ $(CFLAGS)

.PRECIOUS: $(ODIR)/%.s

# Create game
game.nes: $(DEPS)
	$(LD) $(LDFLAGS) -o $(ODIR)/$@ $^ $(BUILD_FILES) $(LABELS) $(EXTRAS)
	python scripts/fceux_symbols.py $(ODIR)/game.labels.txt 
	
.PHONY: maps

# Build process - tmx to h files
$(ASSETS_MAPS)/%.h: $(ASSETS_MAPS)/%.tmx
	python scripts/convert_tiledxml_2_gamefile.py $^ $@
	@echo Making $<

maps: $(patsubst %.tmx,%.h,$(wildcard $(ASSETS_MAPS)/*.tmx))
	@echo $@ $^

.PHONY: clean

clean:
	$(REMOVE) $(ODIR)