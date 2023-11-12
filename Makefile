#
# A Makefile that compiles all .c and .s files in "src" and "res"
# subdirectories and places the output in a "obj" subdirectory
#

# .gb ROM file
PROJECTNAME = Snake
OBJDIR      = obj
BIN         = $(OBJDIR)/$(PROJECTNAME).gb

# Bank 0
SRCDIR      = src
ECSDIR      = $(SRCDIR)/ecs
RESDIR      = res
CSOURCES    = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(ECSDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.c)))
ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.s))) $(foreach dir,$(ECSDIR),$(notdir $(wildcard $(dir)/*.s)))
OBJS        = $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.s=$(OBJDIR)/%_asm.o) ext/hUGEDriver/obj/hUGEDriver.o

# Bank 1: Titlescreen
BANK1_SRCDIR       := $(SRCDIR)/titlescreen
BANK1_RESDIR       := $(RESDIR)/titlescreen
BANK1_CSOURCES     := $(foreach dir,$(BANK1_SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(BANK1_RESDIR),$(notdir $(wildcard $(dir)/*.c)))
BANK1_OBJS         := $(BANK1_CSOURCES:%.c=$(OBJDIR)/%.o)
BANK1_TXT_SOURCES  := $(foreach dir,$(BANK1_RESDIR),$(notdir $(wildcard $(dir)/*.txt)))
BANK1_TXT_INCLUDES := $(BANK1_TXT_SOURCES:%.txt=$(BANK1_RESDIR)/%.inc)

# Bank 2: Level 1: Garden
BANK2_SRCDIR       := $(SRCDIR)/level1_garden
BANK2_RESDIR       := $(RESDIR)/level1_garden
BANK2_CSOURCES     := $(foreach dir,$(BANK2_SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(BANK2_RESDIR),$(notdir $(wildcard $(dir)/*.c)))
BANK2_OBJS         := $(BANK2_CSOURCES:%.c=$(OBJDIR)/%.o)
BANK2_TXT_SOURCES  := $(foreach dir,$(BANK2_RESDIR),$(notdir $(wildcard $(dir)/*.txt)))
BANK2_TXT_INCLUDES := $(BANK2_TXT_SOURCES:%.txt=$(BANK2_RESDIR)/%.inc)

# Bank 3: Level 2: Clouds
BANK3_SRCDIR       := $(SRCDIR)/level2_clouds
BANK3_RESDIR       := $(RESDIR)/level2_clouds
BANK3_CSOURCES     := $(foreach dir,$(BANK3_SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(BANK3_RESDIR),$(notdir $(wildcard $(dir)/*.c)))
BANK3_OBJS         := $(BANK3_CSOURCES:%.c=$(OBJDIR)/%.o)
BANK3_TXT_SOURCES  := $(foreach dir,$(BANK3_RESDIR),$(notdir $(wildcard $(dir)/*.txt)))
BANK3_TXT_INCLUDES := $(BANK3_TXT_SOURCES:%.txt=$(BANK3_RESDIR)/%.inc)

# Bank 4: Level 3: Underground
BANK4_SRCDIR       := $(SRCDIR)/level3_underground
BANK4_RESDIR       := $(RESDIR)/level3_underground
BANK4_CSOURCES     := $(foreach dir,$(BANK4_SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(BANK4_RESDIR),$(notdir $(wildcard $(dir)/*.c)))
BANK4_OBJS         := $(BANK4_CSOURCES:%.c=$(OBJDIR)/%.o)
BANK4_TXT_SOURCES  := $(foreach dir,$(BANK4_RESDIR),$(notdir $(wildcard $(dir)/*.txt)))
BANK4_TXT_INCLUDES := $(BANK4_TXT_SOURCES:%.txt=$(BANK4_RESDIR)/%.inc)

# Compiler
LCC = $(GBDK_HOME)/bin/lcc
LCCFLAGS += -v -Wf--verbose -Wf--Werror -Wl-m

all: LCCFLAGS += -Wf--opt-code-speed -Wf--max-allocs-per-node500000
all: prepare texts $(BIN)

debug: LCCFLAGS += -debug -DDEBUG -Wf--opt-code-size -Wf--max-allocs-per-node4000
debug: prepare texts $(BIN)

# Bank 0

# Compile .c files to .o object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<
$(OBJDIR)/%.o: $(ECSDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<
$(OBJDIR)/%_asm.o: $(ECSDIR)/%.s
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .c files in "res/" to .o object files
$(OBJDIR)/%.o: $(RESDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .s assembly files in "src/" to .o object files
$(OBJDIR)/%.o: $(SRCDIR)/%.s
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# If needed, compile .c files in "src/" to .s assembly files
# (not required if .c is compiled directly to .o)
$(OBJDIR)/%.s: $(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -S -o $@ $<

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

# Bank 3
$(OBJDIR)/%.o: $(BANK3_SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<
$(OBJDIR)/%.o: $(BANK3_RESDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Bank 4
$(OBJDIR)/%.o: $(BANK4_SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<
$(OBJDIR)/%.o: $(BANK4_RESDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

texts: $(BANK1_TXT_INCLUDES) $(BANK2_TXT_INCLUDES) $(BANK3_TXT_INCLUDES) $(BANK4_TXT_INCLUDES)

$(BANK1_RESDIR)/%.inc: $(BANK1_RESDIR)/%.txt
	python3 tools/gentxt.py $< > $@
$(BANK2_RESDIR)/%.inc: $(BANK2_RESDIR)/%.txt
	python3 tools/gentxt.py $< > $@
$(BANK3_RESDIR)/%.inc: $(BANK3_RESDIR)/%.txt
	python3 tools/gentxt.py $< > $@
$(BANK4_RESDIR)/%.inc: $(BANK4_RESDIR)/%.txt
	python3 tools/gentxt.py $< > $@

# Link the compiled object files into a .gb ROM file
$(BIN): $(OBJS) $(BANK1_OBJS) $(BANK2_OBJS) $(BANK3_OBJS) $(BANK4_OBJS)
	$(LCC) $(LCCFLAGS) -Wl-yt0x1A -Wl-yo8 -Wl-ya4 -o $@ $(OBJS) $(BANK1_OBJS) $(BANK2_OBJS) $(BANK3_OBJS) $(BANK4_OBJS)

prepare:
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJDIR)/*.* $(BANK1_TXT_INCLUDES) $(BANK2_TXT_INCLUDES) $(BANK3_TXT_INCLUDES) $(BANK4_TXT_INCLUDES)

# Debug
print-%:
	@echo $* = $($*)
