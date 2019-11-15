# build a program from 2 files and one shared header

# OBJECT dependencies for the game to be correctly compiled
_DEPS = main.o crt0.o

# Compile C to asm
CC = cc65
# Compile Asm to Object file
CA = ca65
# Link object files in a single final file
LD = ld65

CC65FLAGS = --add-source --all-cdecl --eagerly-inline-funcs --inline-stdfuncs --check-stack -t nes
CFLAGS = -g 
LDFLAGS = -C nrom_32k_vert.cfg
BUILD_FILES = nes.lib -Ln

ODIR=output
_LABELS = labels.txt
DEPS = $(patsubst %,$(ODIR)/%,$(_DEPS))
LABELS = $(patsubst %,$(ODIR)/%,$(_LABELS))

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
	$(LD) $(LDFLAGS) -o $(ODIR)/$@ $^ $(BUILD_FILES) $(LABELS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.s $(ODIR)/*.nes $(ODIR)/*.txt

clean-misc:
	rm -f $(ODIR)/*.o $(ODIR)/*.s $(ODIR)/*.txt