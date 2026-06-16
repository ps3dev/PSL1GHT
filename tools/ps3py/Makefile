UNAME	:=	$(shell uname -s)

ifeq ($(strip $(PS3DEV)),)
  ifeq ($(strip $(DEVKITPS3)),)
    export PS3DEV := /usr/local/ps3dev
  else
    export PS3DEV := $(DEVKITPS3)
  endif
endif

ifneq (,$(findstring MINGW,$(UNAME)))
	PLATFORM	:= win32
	COMPILER	:= --compiler=mingw32
	OS			:= win32
endif

all: pkgcrypt.so

pkgcrypt.so: crypt.c setup.py
	@`./find-python` setup.py build_ext $(COMPILER)
	@cp build/lib.*/pkgcrypt.* .

clean:
	@echo clean	...
	@rm -rf build pkgcrypt.*

install: all
	@[ -d $(PS3DEV)/bin ] || mkdir -p $(PS3DEV)/bin
	@echo Installing pkgcrypt.* fself.py Struct.py sfo.py pkg.py
	@install -m 644 pkgcrypt.* $(PS3DEV)/bin
	@./install-scripts $(PS3DEV)/bin/ fself.py Struct.py sfo.py pkg.py
