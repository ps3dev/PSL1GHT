CC			:=	$(PREFIX)gcc
CXX			:=	$(PREFIX)g++
AR			:=	$(PREFIX)ar
OBJCOPY		:=	$(PREFIX)objcopy

UNAME		:=	$(shell uname -s)

CFLAGS		:=	$(DEBUGFLAGS) -O2
CFLAGS		+=	$(INCLUDES)
CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions
LDFLAGS		=	$(DEBUGFLAGS)

ifeq ($(strip $(PS3DEV)),)
  ifeq ($(strip $(DEVKITPS3)),)
    export PS3DEV := /usr/local/ps3dev
  else
    export PS3DEV := $(DEVKITPS3)
  endif
endif

ifneq (,$(findstring MINGW,$(UNAME)))
	PLATFORM	:= win32
	POSTFIX		:= .exe
	CFLAGS		+= -mno-cygwin
	LDFLAGS		+= -mno-cygwin -s -lgmp -lcrypto -lz
	OS			:= win32
endif

ifneq (,$(findstring CYGWIN,$(UNAME)))
	LDFLAGS		+= -s -lgmp -lcrypto -lz
	POSTFIX		:= .exe
	OS			:= win32
endif

ifneq (,$(findstring Darwin,$(UNAME)))
	SDK			:= $(shell xcrun --show-sdk-path)
	OSX_MIN 	:= $(shell defaults read $(shell xcrun --show-sdk-platform-path 2> /dev/null)/Info MinimumSDKVersion 2> /dev/null || xcrun --show-sdk-version 2> /dev/null || echo 10.4)
	OSXCFLAGS	:= -mmacosx-version-min=$(OSX_MIN)
	OSXCXXFLAGS	:=	$(OSXCFLAGS)
	CXXFLAGS	+=	-fvisibility=hidden
	CFLAGS	+= -I/usr/local/include -I/usr/local/opt/libelf/include/libelf -I/usr/local/opt/openssl/include
	LDFLAGS		+= -mmacosx-version-min=$(OSX_MIN) -Wl,-syslibroot,$(SDK) -L/opt/local/lib -L/usr/local/opt/openssl/lib -lgmp -lcrypto -lz

  ifneq (,$(findstring arm,$(shell uname -p)))
	CFLAGS	    += -I/opt/homebrew/include -I/opt/homebrew/opt/libelf/include/libelf -I/opt/homebrew/opt/openssl/include
	LDFLAGS		+= -L/opt/homebrew/lib -L/opt/homebrew/opt/openssl/lib
  endif

endif

ifneq (,$(findstring BSD,$(UNAME)))
	CFLAGS += -I/usr/local/include
	LDFLAGS += -L/usr/local/lib -lgmp -lcrypto -lz
endif

ifneq (,$(findstring SunOS,$(UNAME)))
        CFLAGS += -I/opt/csw/include
        LDFLAGS += -L/opt/csw/lib -R/opt/csw/lib -lgmp -lcrypto -lz
endif

ifneq (,$(findstring Linux,$(UNAME)))
	LDFLAGS += -lgmp -lcrypto -lz
	OS		:= Linux
endif

TARGETS	:=	make_self$(POSTFIX) make_self_npdrm$(POSTFIX) package_finalize$(POSTFIX)

all: $(TARGETS)

make_self$(POSTFIX): make_self.c
	@echo building ... $(notdir $@)
	@$(CC) $< $(CFLAGS) $(LDFLAGS) -o $@

make_self_npdrm$(POSTFIX): make_self.c
	@echo building ... $(notdir $@)
	@$(CC) $< $(CFLAGS) $(LDFLAGS) -DNPDRM -o $@

package_finalize$(POSTFIX): package_finalize.c
	@echo building ... $(notdir $@)
	@$(CC) $< $(CFLAGS) $(LDFLAGS) -o $@

install: $(TARGETS)
	@echo Installing $(TARGETS)
	@[ -d $(PS3DEV)/bin ] || mkdir -p $(PS3DEV)/bin
	@install -m 755 $(TARGETS) $(PS3DEV)/bin

clean:
	@echo clean ...
	@rm -f $(TARGETS)

.PHONY: install clean
