

# ------------------------------------------------
# Generic Makefile
#
# Author: yanick.rochon@gmail.com
# Date  : 2011-08-10
#
# Changelog :
#   2010-11-05 - first version
#   2011-08-10 - added structure : sources, objects, binaries
#				thanks to http://stackoverflow.com/users/128940/beta
#   2017-04-24 - changed order of linker params
# ------------------------------------------------

# -------------------
# CCCP Makefile
# Modified by: PKD
#--------------------




CC = gcc
CPP = g++

ODIR = obj
SDIR = src

EXEOUT = rt

CFLAGS = -Wall -g -fPIC -O2 -Wextra -L./bin -Iinclude

LIBS = lib/* -lm -lSDL2

# change these to proper directories where each file should be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin
INCDIR   = include

DEVDIR = dev


SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)




all: $(BINDIR)/$(EXEOUT)
	@echo "BUILD SUCESSFUL"

$(BINDIR)/$(EXEOUT): $(OBJECTS)
	@$(CC) $(OBJECTS) $(LIBS) $(LFLAGS) -o $@ 
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c

	@if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi
	@if [ ! -d $(BINDIR) ]; then mkdir $(BINDIR); fi

	$(CC) $(CFLAGS) -D OBJ_COUNT=$(OBJ_COUNT) -c $< -o $@
	@echo "Compiled "$<" successfully!"

clean:
	rm -f $(ODIR)/*.o $(BINDIR)/*



	


