.SUFFIXES:

ifeq ($(strip $(PS3DEV)),)
  ifeq ($(strip $(DEVKITPS3)),)
    export PS3DEV := /usr/local/ps3dev
  else
    export PS3DEV := $(DEVKITPS3)
  endif
endif

ifneq (,$(findstring MINGW,$(shell uname -s)))
	host		:= win32
endif

ifneq (,$(findstring Linux,$(shell uname -s)))
	host		:= linux
endif

ifneq (,$(findstring Darwin,$(shell uname -s)))
	host		:= osx
endif

all:
	@$(MAKE) -C geohot --no-print-directory
	@$(MAKE) -C sprxlinker --no-print-directory
	@$(MAKE) -C ps3load --no-print-directory
	@$(MAKE) -C generic --no-print-directory
	@$(MAKE) -C cgcomp --no-print-directory
	@$(MAKE) -C ps3py --no-print-directory
	@$(MAKE) -C raw2h --no-print-directory

install-socat:
	@[ -d $(PS3DEV)/bin ] || mkdir -p $(PS3DEV)/bin
	@cp -frv $(CURDIR)/socat/$(host)/* $(PS3DEV)/bin

install:
	@$(MAKE) -C geohot install --no-print-directory
	@$(MAKE) -C sprxlinker install --no-print-directory
	@$(MAKE) -C ps3load install --no-print-directory
	@$(MAKE) -C generic install --no-print-directory
	@$(MAKE) -C cgcomp install --no-print-directory
	@$(MAKE) -C ps3py install --no-print-directory
	@$(MAKE) -C raw2h install --no-print-directory

clean:
	@$(MAKE) -C geohot clean --no-print-directory
	@$(MAKE) -C sprxlinker clean --no-print-directory
	@$(MAKE) -C ps3load clean --no-print-directory
	@$(MAKE) -C generic clean --no-print-directory
	@$(MAKE) -C cgcomp clean --no-print-directory
	@$(MAKE) -C ps3py clean --no-print-directory
	@$(MAKE) -C raw2h clean --no-print-directory

.PHONY: all clean install install-socat
