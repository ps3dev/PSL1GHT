.SUFFIXES:
ifeq ($(strip $(PSL1GHT)),)
$(error "Please set PSL1GHT in your environment. export PSL1GHT=<path>")
endif

TITLE       :=  PS Eye Sample - PSL1GHT
APPID       :=  PSEYETEST
CONTENTID   :=  UP0001-$(APPID)_00-0000000000000000

include $(PSL1GHT)/ppu_rules

TARGET      :=  $(notdir $(CURDIR))
BUILD       :=  build
SOURCES     :=  source
DATA        :=  data
INCLUDES    :=  include

LIBS        :=  -lrsx -lgcm_sys -lio -lsysutil -lrt -llv2 -lsysmodule -lcamera -lz -lm -llv2dbg

CFLAGS      =   -O2 -Wall -mcpu=cell $(MACHDEP) $(INCLUDE)
LDFLAGS     =   $(MACHDEP) -Wl,-Map,$(notdir $@).map
LIBDIRS     :=  $(CURDIR)

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT   :=  $(CURDIR)/$(TARGET)
export GAMEROOT :=  $(CURDIR)

export VPATH   :=  $(shell find $(SOURCES) $(DATA) -type d 2>/dev/null | sed "s|^|$(CURDIR)/|")
export DEPSDIR :=  $(CURDIR)/$(BUILD)
export BUILDDIR := $(CURDIR)/$(BUILD)

CFILES      :=  $(shell find $(SOURCES) -type f -name '*.c')
BINFILES    :=  $(shell find $(DATA) -type f -name '*.bin' 2>/dev/null)

export LD   :=  $(CC)
export OFILES := $(addsuffix .o,$(BINFILES)) $(CFILES:.c=.c.o)

export INCLUDE := $(foreach dir,$(INCLUDES), -I$(CURDIR)/$(dir)) \
                  $(foreach dir,$(shell find $(INCLUDES) $(SOURCES) -type d 2>/dev/null),-I$(CURDIR)/$(dir)) \
                  $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                  $(LIBPSL1GHT_INC) \
                  -I$(CURDIR)/$(BUILD) -I$(PORTLIBS)/include

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
                   $(LIBPSL1GHT_LIB) -L$(PORTLIBS)/lib

.PHONY: $(BUILD) clean run pkg npdrm

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).self $(OUTPUT).fake.self $(OUTPUT).pkg $(OUTPUT).gnpdrm.pkg EBOOT.BIN

run:
	ps3load $(OUTPUT).self

pkg: $(BUILD) $(OUTPUT).pkg

npdrm: $(BUILD)
	@$(SELF_NPDRM) $(SCETOOL_FLAGS) $(BUILDDIR)/$(basename $(notdir $(OUTPUT))).elf $(BUILDDIR)/../EBOOT.BIN --np-content-id=$(CONTENTID)

else

DEPENDS := $(addprefix $(DEPSDIR)/,$(filter-out %.bin.d,$(OFILES:.o=.d)))

$(OUTPUT).self: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)

%.bin.o: %.bin
	@echo $(notdir $<)
	@$(bin2o)

$(SOURCES)/%.c.o: $(GAMEROOT)/$(SOURCES)/%.c
	@echo $(notdir $<)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(DATA)/%.bin.o: $(GAMEROOT)/$(DATA)/%.bin
	@echo $(notdir $<)
	@mkdir -p $(dir $@)
	@$(bin2o)

-include $(DEPENDS)

endif
