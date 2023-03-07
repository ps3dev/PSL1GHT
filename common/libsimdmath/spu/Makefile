#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(PSL1GHT)),)
$(error "Please set PSL1GHT in your environment. export PSL1GHT=<path>")
endif

include	$(PSL1GHT)/spu_rules

BUILD		:=	build

#---------------------------------------------------------------------------------
ifeq ($(strip $(PLATFORM)),)
#---------------------------------------------------------------------------------
export BASEDIR		:=	$(CURDIR)
export DEPS			:=	$(BASEDIR)/deps
export LIBS			:=	$(BASEDIR)/lib

#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------

export LIBDIR		:=	$(LIBS)/$(PLATFORM)
export DEPSDIR		:=	$(DEPS)/$(PLATFORM)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

LIBRARY		:= $(LIBDIR)/libsimdmath

#---------------------------------------------------------------------------------
DEFINCS		:=	-I$(BASEDIR) -I$(BASEDIR)/../common -I$(BASEDIR)/../common/simdmath
INCLUDES	:=	$(DEFINCS)

CFLAGS		:=	-O2 -Wall $(MACHDEP) -DLIBSIMDMATH_INTERNAL $(INCLUDES)
ASFLAGS		:=	$(MACHDEP) -D__ASSEMBLY__ $(INCLUDES)

#---------------------------------------------------------------------------------
VPATH :=	$(BASEDIR) \
			$(BASEDIR)/../common

#---------------------------------------------------------------------------------
OBJS		:= \
			absi4.o acosf4.o asinf4.o atan2f4.o atanf4.o cbrtf4.o ceilf4.o copysignf4.o cosf4.o \
			divf4.o divi4.o exp2f4.o expf4.o expm1f4.o fabsd2.o fabsf4.o fdimf4.o floorf4.o fmaf4.o \
			fmaxf4.o fminf4.o fmodf4.o frexpf4.o hypotf4.o ilogbf4.o isnanf4.o isnand2.o isinff4.o isinfd2.o \
			is0denormf4.o is0denormd2.o ldexpf4.o log1pf4.o log2f4.o log10f4.o recipd2.o divd2.o tand2.o \
			logbf4.o modff4.o negatef4.o negatei4.o powf4.o recipf4.o rsqrtf4.o sincosf4.o sinf4.o \
			sqrtf4.o tanf4.o truncf4.o sqrtd2.o isgreaterd2.o sind2.o sincosd2.o signbitf4.o signbitd2.o \
			rsqrtd2.o remainderf4.o  copysignd2.o negated2.o frexpd2.o cosd2.o hypotd2.o logf4.o ilogbd2.o \
			logbf4.o logbd2.o llroundd2.o llroundf4.o llrintf4.o isequalf4.o isequald2.o islessgreaterf4.o \
			islessgreaterd2.o islessf4.o islessd2.o isgreaterf4.o isgreaterd2.o islessequalf4.o islessequald2.o \
			isgreaterequalf4.o isgreaterequald2.o isfinitef4.o isfinited2.o isnormalf4.o isnormald2.o isunorderedf4.o \
			isunorderedd2.o llrintd2.o roundf4.o rintf4.o irintf4.o iroundf4.o fmad2.o fmaxd2.o fmind2.o fdimd2.o \
			nextafterd2.o fpclassifyf4.o fpclassifyd2.o nearbyintd2.o nextafterf4.o nearbyintf4.o llabsi2.o truncd2.o \
			llabsi2.o truncd2.o roundd2.o rintd2.o negatell2.o divu4.o modfd2.o lldivu2.o ceild2.o floord2.o ldexpd2.o \
			scalbnf4.o scalbllnd2.o lldivi2.o remquof4.o remquod2.o fmodd2.o remainderd2.o

all: spu

#---------------------------------------------------------------------------------
spu:
#---------------------------------------------------------------------------------
	@[ -d $(LIBS)/spu ] || mkdir -p $(LIBS)/spu
	@[ -d $(DEPS)/spu ] || mkdir -p $(DEPS)/spu
	@[ -d spu ] || mkdir -p spu
	@$(MAKE) PLATFORM=spu lib -C spu -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
install-header:
#---------------------------------------------------------------------------------
	@[ -d $(DESTDIR)$(PSL1GHT)/spu/include/simdmath ] || mkdir -p $(DESTDIR)$(PSL1GHT)/spu/include/simdmath
	@cp -frv $(CURDIR)/../common/simdmath.h $(DESTDIR)$(PSL1GHT)/spu/include/simdmath

#---------------------------------------------------------------------------------
install: all install-header
#---------------------------------------------------------------------------------
	@[ -d $(DESTDIR)$(PSL1GHT)/spu/lib ] || mkdir -p $(DESTDIR)$(PSL1GHT)/spu/lib
	@cp -frv $(CURDIR)/lib/spu/*.a $(DESTDIR)$(PSL1GHT)/spu/lib

#---------------------------------------------------------------------------------
$(LIBRARY).a: $(OBJS)
#---------------------------------------------------------------------------------

.PHONY: lib spu clean install

#---------------------------------------------------------------------------------
lib: $(LIBRARY).a
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
clean:
#---------------------------------------------------------------------------------
	@echo clean ...
	@rm -fr spu
	@rm -fr $(DEPS)
	@rm -fr $(LIBS)

-include $(DEPSDIR)/*.d
