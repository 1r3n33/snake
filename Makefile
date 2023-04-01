#
# A Makefile that compiles all .c and .s files in "src" and "res" 
# subdirectories and places the output in a "obj" subdirectory
#

# You can set the name of the .gb ROM file here
PROJECTNAME	= Snake

SRCDIR      = src
OBJDIR      = obj
RESDIR      = res
BINS	    = $(OBJDIR)/$(PROJECTNAME).gb
CSOURCES    = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.c)))
ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.s)))
OBJS      	= $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.s=$(OBJDIR)/%.o) ext/hUGEDriver/obj/hUGEDriver.o

# Bank 1: Titlescreen
BANK1_SRCDIR 	:= $(SRCDIR)/titlescreen
BANK1_RESDIR 	:= $(RESDIR)/titlescreen
BANK1_CSOURCES	:= $(foreach dir,$(BANK1_SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(BANK1_RESDIR),$(notdir $(wildcard $(dir)/*.c)))
BANK1_OBJS		:= $(BANK1_CSOURCES:%.c=$(OBJDIR)/%.o)

# Bank 2: Level 1: Garden
BANK2_SRCDIR	:= $(SRCDIR)/level1_garden
BANK2_RESDIR 	:= $(RESDIR)/level1_garden
BANK2_CSOURCES	:= $(foreach dir,$(BANK2_SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(BANK2_RESDIR),$(notdir $(wildcard $(dir)/*.c)))
BANK2_OBJS		:= $(BANK2_CSOURCES:%.c=$(OBJDIR)/%.o)

# Compiler
LCC = $(GBDK_HOME)/bin/lcc
LCCFLAGS += -v -debug -Wf--opt-code-size -Wf--max-allocs-per-node300000 -Wf--verbose -Wf--Werror -Wl-m

all:	prepare $(BINS)

# Compile .c files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .c files in "res/" to .o object files
$(OBJDIR)/%.o:	$(RESDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .s assembly files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.s
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# If needed, compile .c files in "src/" to .s assembly files
# (not required if .c is compiled directly to .o)
$(OBJDIR)/%.s:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -S -o $@ $<

# Bank 0

# Bank 1
$(OBJDIR)/%.o: $(BANK1_SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -Wf-bo1 -Wf-ba1 -c -o $@ $<
$(OBJDIR)/%.o: $(BANK1_RESDIR)/%.c
	$(LCC) $(LCCFLAGS) -Wf-bo1 -Wf-ba1 -c -o $@ $<

# Bank 2
$(OBJDIR)/%.o: $(BANK2_SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -Wf-bo2 -Wf-ba2 -c -o $@ $<
$(OBJDIR)/%.o: $(BANK2_RESDIR)/%.c
	$(LCC) $(LCCFLAGS) -Wf-bo2 -Wf-ba2 -c -o $@ $<

# Link the compiled object files into a .gb ROM file
$(BINS): $(OBJS) $(BANK1_OBJS) $(BANK2_OBJS)
	$(LCC) $(LCCFLAGS) -Wl-yt0x1A -Wl-yo4 -Wl-ya4 -o $@ $(OBJS) $(BANK1_OBJS) $(BANK2_OBJS)

prepare:
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJDIR)/*.*
