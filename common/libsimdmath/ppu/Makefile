#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(PSL1GHT)),)
$(error "Please set PSL1GHT in your environment. export PSL1GHT=<path>")
endif

include	$(PSL1GHT)/ppu_rules

BUILD		:=	build

#---------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),)
#---------------------------------------------------------------------------------
export BASEDIR		:= $(CURDIR)
export DEPS			:= $(BASEDIR)/deps
export LIBS			:= $(BASEDIR)/lib

#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------

export LIBDIR		:= $(LIBS)/$(PLATFORM)
export DEPSDIR		:= $(DEPS)/$(PLATFORM)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

LIBRARY		:= $(LIBDIR)/libsimdmath

#---------------------------------------------------------------------------------
DEFINCS		:= -I$(BASEDIR) -I$(BASEDIR)/../common -I$(BASEDIR)/../common/simdmath
INCLUDES	:=	$(DEFINCS)

CFLAGS		:= -O2 -mregnames -Wall -mcpu=cell $(MACHDEP) -DLIBSIMDMATH_INTERNAL $(INCLUDES) -Wa,-mcell
ASFLAGS		:=	$(MACHDEP) -mregnames -mcpu=cell -D__ASSEMBLY__ -Wa,-mcell $(INCLUDES)

#---------------------------------------------------------------------------------
VPATH :=	$(BASEDIR) \
			$(BASEDIR)/../common

#---------------------------------------------------------------------------------
OBJS		:= \
			absi4.o acosf4.o asinf4.o atan2f4.o atanf4.o cbrtf4.o ceilf4.o copysignf4.o cosf4.o \
			divf4.o divi4.o exp2f4.o expf4.o expm1f4.o fabsf4.o fdimf4.o floorf4.o fmaf4.o fmaxf4.o \
			fminf4.o fmodf4.o frexpf4.o hypotf4.o ilogbf4.o ldexpf4.o logf4.o log1pf4.o log2f4.o \
			log10f4.o logbf4.o modff4.o negatef4.o negatei4.o powf4.o recipf4.o rsqrtf4.o sincosf4.o \
			sinf4.o sqrtf4.o tanf4.o truncf4.o

all: ppu

#---------------------------------------------------------------------------------
ppu:
#---------------------------------------------------------------------------------
	@[ -d $(LIBS)/ppu ] || mkdir -p $(LIBS)/ppu
	@[ -d $(DEPS)/ppu ] || mkdir -p $(DEPS)/ppu
	@[ -d ppu ] || mkdir -p ppu
	@$(MAKE) PLATFORM=ppu lib -C ppu -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
install-header:
#---------------------------------------------------------------------------------
	@[ -d $(PSL1GHT)/ppu/include/simdmath ] || mkdir -p $(PSL1GHT)/ppu/include/simdmath
	@cp -frv $(CURDIR)/../common/simdmath.h $(PSL1GHT)/ppu/include/simdmath

#---------------------------------------------------------------------------------
install: all install-header
#---------------------------------------------------------------------------------
	@[ -d $(PSL1GHT)/ppu/lib ] || mkdir -p $(PSL1GHT)/ppu/lib
	@cp -frv $(CURDIR)/lib/ppu/*.a $(PSL1GHT)/ppu/lib

#---------------------------------------------------------------------------------
$(LIBRARY).a: $(OBJS)
#---------------------------------------------------------------------------------

.PHONY: lib ppu clean install

#---------------------------------------------------------------------------------
lib: $(LIBRARY).a
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
clean:
#---------------------------------------------------------------------------------
	@echo clean ...
	@rm -fr ppu
	@rm -fr $(DEPS)
	@rm -fr $(LIBS)

-include $(DEPSDIR)/*.d
