UNAME	:=	$(shell uname -s)

ifeq ($(strip $(PS3DEV)),)
  ifeq ($(strip $(DEVKITPS3)),)
    export PS3DEV := /usr/local/ps3dev
  else
    export PS3DEV := $(DEVKITPS3)
  endif
endif

CC		:=	gcc
CFLAGS	:=	-Wall -O2

ifneq (,$(findstring MINGW,$(UNAME)))
	PLATFORM	:= win32
	COMPILER	:= --compiler=mingw32
	OS			:= win32
	EXEEXT		:= .exe
endif

ifneq (,$(findstring Darwin,$(UNAME)))
	SDK		:= $(shell xcrun --show-sdk-path)
	OSX_MIN := $(shell defaults read $(shell xcrun --show-sdk-platform-path 2>/dev/null)/Info MinimumSDKVersion 2>/dev/null || xcrun --show-sdk-version 2>/dev/null || echo 10.4)
	CFLAGS	+= -mmacosx-version-min=$(OSX_MIN) -isysroot $(SDK)
endif

SFO_TARGET	:= sfo$(EXEEXT)
PKG_TARGET	:= pkg$(EXEEXT)

all: $(SFO_TARGET) $(PKG_TARGET)

$(SFO_TARGET): sfo.c
	@echo building sfo ...
	@$(CC) $(CFLAGS) $< -o $@

$(PKG_TARGET): pkg.c sha1.c
	@echo building pkg ...
	@$(CC) $(CFLAGS) $^ -o $@

clean:
	@echo clean	...
	@rm -rf build $(SFO_TARGET) $(PKG_TARGET)

install: all
	@[ -d $(PS3DEV)/bin ] || mkdir -p $(PS3DEV)/bin
	@echo Installing $(SFO_TARGET) $(PKG_TARGET) ICON0.PNG sfo.xml
	@install -m 755 $(SFO_TARGET) $(PKG_TARGET) $(PS3DEV)/bin
	@install -m 644 ICON0.PNG sfo.xml $(PS3DEV)/bin
