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

ifeq (${OS},Windows_NT)
		REMOVE = C:/cygwin64/bin/rm -f
		MKDIR_P = mkdir
else
        REMOVE = rm -f
		MKDIR_P = mkdir -p
endif

# process that creates the output folder if not present

ifeq (,$(wildcard $(@D)))
    	dir_guard=
else
		dir_guard=$(MKDIR_P) $(@D)	
endif

# Build process - c to asm files
$(ODIR)/%.s: %.c
	$(dir_guard)
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
	$(REMOVE) $(ODIR)/*.o $(ODIR)/*.s $(ODIR)/*.nes $(ODIR)/*.txt

clean-misc:
	$(REMOVE) $(ODIR)/*.o $(ODIR)/*.s $(ODIR)/*.txt
