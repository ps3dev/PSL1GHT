#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(PSL1GHT)),)
$(error "Please set PSL1GHT in your environment. export PSL1GHT=<path>")
endif

BUILD		:=	build

#---------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),)
#---------------------------------------------------------------------------------
export BASEDIR		:= $(CURDIR)
export DEPF			:= $(BASEDIR)/deps
export LIBF			:= $(BASEDIR)/lib

#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------

export LIBDIR		:= $(LIBF)/$(PLATFORM)
export DEPSDIR		:= $(DEPF)/$(PLATFORM)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

include	$(PSL1GHT)/spu_rules

LD				:= $(CC)
LIBRARYBASE		:= $(LIBDIR)/libspumars
LIBRARYTASK		:= $(LIBDIR)/libspumarstask

KERNELMODULE	:= $(LIBF)/mars_kernel
TASKMODULE		:= $(LIBF)/mars_task_module
JQMODULE		:= $(LIBF)/mars_jq_module

#---------------------------------------------------------------------------------
INCLUDES	:= -I$(BASEDIR) \
			   -I$(BASEDIR)/../include/common \
			   -I$(BASEDIR)/../include/spu \
			   -I$(BASEDIR)/../base/common \
			   -I$(BASEDIR)/../task/common \
			   -I$(BASEDIR)/../jq/common \
			   -I$(BASEDIR)/../../../spu/include

LIBS 		:= -lsputhread
LIBPATHS	:= -L$(BASEDIR)/../../../spu/libsputhread/lib/spu

CFLAGS		:= -Os -Wall $(MACHDEP) -DLIBSPUMARS_INTERNAL $(INCLUDES)
ASFLAGS		:= $(MACHDEP) -D__ASSEMBLY__ $(INCLUDES)

#---------------------------------------------------------------------------------
VPATH :=	$(BASEDIR) \
			$(BASEDIR)/base/lib \
			$(BASEDIR)/base/kernel \
			$(BASEDIR)/task/lib \
			$(BASEDIR)/task/module \
			$(BASEDIR)/jq/lib \
			$(BASEDIR)/jq/module

#---------------------------------------------------------------------------------
OBJS_LIB_BASE	:= module.o

OBJS_LIB_TASK	:= task.o task_barrier.o task_event_flag.o \
			       task_queue.o task_semaphore.o task_signal.o
			   
OBJS_KERNEL		:= kernel_crt.o switch.o kernel.o dma.o mutex.o

OBJS_TASK		:= task_switch.o task_module.o

OBJS_JQ			:= jq_switch.o jq_module.o
			
all: spu

#---------------------------------------------------------------------------------
spu:
#---------------------------------------------------------------------------------
	@[ -d $(LIBF)/spu ] || mkdir -p $(LIBF)/spu
	@[ -d $(DEPF)/spu ] || mkdir -p $(DEPF)/spu
	@[ -d spu ] || mkdir -p spu
	@$(MAKE) PLATFORM=spu libs -C spu -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
install-header:
#---------------------------------------------------------------------------------
	@[ -d $(DESTDIR)$(PSL1GHT)/spu/include ] || mkdir -p $(DESTDIR)$(PSL1GHT)/spu/include
	@cp -frv $(CURDIR)/../include/common/mars $(DESTDIR)$(PSL1GHT)/spu/include
	@cp -frv $(CURDIR)/../include/spu/mars $(DESTDIR)$(PSL1GHT)/spu/include

#---------------------------------------------------------------------------------
install: all install-header
#---------------------------------------------------------------------------------
	@[ -d $(DESTDIR)$(PSL1GHT)/spu/lib ] || mkdir -p $(DESTDIR)$(PSL1GHT)/spu/lib
	@cp -frv $(CURDIR)/lib/spu/*.a $(DESTDIR)$(PSL1GHT)/spu/lib

#---------------------------------------------------------------------------------
DEPENDS	:=	$(OBJS_KERNEL:.o=.d) $(OBJS_LIB_BASE:.o=.d) \
			$(OBJS_TASK:.o=.d) $(OBJS_LIB_TASK:.o=.d) \
			$(OBJS_JQ:.o=.d)

$(LIBRARYBASE).a: CFLAGS += --param max-inline-insns-single=20
$(LIBRARYBASE).a: $(OBJS_LIB_BASE)

$(LIBRARYTASK).a: CFLAGS += --param max-inline-insns-single=20
$(LIBRARYTASK).a: $(OBJS_LIB_TASK)

$(KERNELMODULE).elf: CFLAGS += -mfixed-range=80-127 -funroll-loops -fschedule-insns
$(KERNELMODULE).elf: $(OBJS_KERNEL)
	@echo linking ... $(notdir $@)
	$(VERB) $(LD)  $^ -nostdlib \
			   -Wl,--defsym=__stack=0x29f0 \
			   -Wl,-Map -Wl,$(CURDIR)/$(notdir $@).map \
			   -Wl,--cref \
			   -Wl,--gc-sections \
			   -Wl,--sort-common \
			   -Wl,--sort-section=alignment \
			   -Wl,--cref \
			   -Wl,-N \
			   -Wl,-s \
			   $(LIBPATHS) $(LIBS) -o $@

$(TASKMODULE).elf: CFLAGS += -mfixed-range=80-127 -funroll-loops -fschedule-insns
$(TASKMODULE).elf: $(OBJS_TASK)
	@echo linking ... $(notdir $@)
	$(VERB) $(LD)  $^ -nostdlib \
			   -Wl,--defsym=__stack=0x39e0 \
			   -Wl,-Ttext-segment=0x2a00 \
			   -Wl,--entry,mars_module_entry -Wl,-u,mars_module_entry \
			   -Wl,-Map -Wl,$(CURDIR)/$(notdir $@).map \
			   -Wl,--gc-sections \
			   -Wl,--sort-common \
			   -Wl,--sort-section=alignment \
			   -Wl,--cref \
			   -Wl,-s \
			   $(LIBPATHS) $(LIBS) -L$(LIBDIR) -lspumars -o $@

$(JQMODULE).elf: CFLAGS += -mfixed-range=80-127 -funroll-loops -fschedule-insns
$(JQMODULE).elf: $(OBJS_JQ)
	@echo linking ... $(notdir $@)
	$(VERB) $(LD)  $^ -nostdlib \
			   -Wl,--defsym=__stack=0x39e0 \
			   -Wl,-Ttext-segment=0x2a00 \
			   -Wl,--entry,mars_module_entry -Wl,-u,mars_module_entry \
			   -Wl,-Map -Wl,$(CURDIR)/$(notdir $@).map \
			   -Wl,--gc-sections \
			   -Wl,--sort-common \
			   -Wl,--sort-section=alignment \
			   -Wl,--cref \
			   -Wl,-s \
			   $(LIBPATHS) $(LIBS) -L$(LIBDIR) -lspumars -o $@

#---------------------------------------------------------------------------------

.PHONY: lib spu install

#---------------------------------------------------------------------------------
libs: libbase libtask libjq
#---------------------------------------------------------------------------------
libbase: $(LIBRARYBASE).a $(KERNELMODULE).elf
libtask: $(LIBRARYTASK).a $(TASKMODULE).elf
libjq: $(JQMODULE).elf
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
clean:
#---------------------------------------------------------------------------------
	@echo clean ...
	@rm -rf spu
	@rm -rf $(DEPF)
	@rm -rf $(LIBF)

-include $(DEPSDIR)/*.d
