#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(PS3DEV)),)
$(error "Please set PS3DEV in your environment. export PS3DEV=<path>")
endif

CC	:=	gcc

CFLAGS	:=	-Wall -Wno-char-subscripts -O2 -s

ifeq ($(strip $(PS3DEV)),)
  ifeq ($(strip $(DEVKITPS3)),)
    export PS3DEV := /usr/local/ps3dev
  else
    export PS3DEV := $(DEVKITPS3)
  endif
endif

ifneq (,$(findstring MINGW,$(shell uname -s)))
	exeext		:= .exe
endif

ifneq (,$(findstring CYGWIN,$(UNAME)))
	exeext		:= .exe
endif

ifneq (,$(findstring Darwin,$(shell uname -s)))
	SDK	:=	$(shell xcrun --show-sdk-path)
	OSX_MIN := $(shell defaults read $(shell xcrun --show-sdk-platform-path 2> /dev/null)/Info MinimumSDKVersion 2> /dev/null || xcrun --show-sdk-version 2> /dev/null || echo 10.4)
	CFLAGS += -mmacosx-version-min=$(OSX_MIN) -isysroot $(SDK) -Wl,-syslibroot,$(SDK)
endif

TARGETS	:=	$(patsubst %.c,%$(exeext),$(wildcard *.c)) \
		$(patsubst %.cpp,%$(exeext),$(wildcard *.cpp))


all:	$(TARGETS)

clean:
	@echo clean ...
	@rm -f $(TARGETS)

%$(exeext)	:: %.c
	@$(CC) $< -o $@ $(CFLAGS)

%$(exeext)	:: %.cpp
	@$(CXX) $< -o $@ $(CFLAGS)

install: all
	@echo Installing $(TARGETS)
	@[ -d $(DESTDIR)$(PS3DEV)/bin ] || mkdir -p $(DESTDIR)$(PS3DEV)/bin
	@install -m 755 $(TARGETS) $(DESTDIR)$(PS3DEV)/bin
