CC := gcc
SRCDIR := src
OBJDIR := build
BINDIR := bin

MAIN := main.c

SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
$(info SOURCES = $(SOURCES))
OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
$(info OBJECTS = $(OBJECTS))

CFLAGS := -std=c11 -O2
INC := -I include/

$(OBJDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(@D)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

all: clean main

clean:
	$(RM) -rf $(BINDIR) $(OBJDIR) *.gcda *.gcno tp2virtual tp2virtual.dSYM

main: $(OBJECTS)
	$(CC) $(CFLAGS) $(INC) $(MAIN) $^ -o tp2virtual