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
	@`./find-python2` setup.py build_ext $(COMPILER)
	@cp build/lib.*/pkgcrypt.* .

clean:
	@echo clean	...
	@rm -rf build pkgcrypt.*

install: all
	@[ -d $(DESTDIR)$(PS3DEV)/bin ] || mkdir -p $(DESTDIR)$(PS3DEV)/bin
	@echo Installing ICON0.PNG sfo.xml pkgcrypt.* fself.py Struct.py sfo.py pkg.py
	@install -m 644 ICON0.PNG sfo.xml pkgcrypt.* $(DESTDIR)$(PS3DEV)/bin
	@./install-scripts $(DESTDIR)$(PS3DEV)/bin/ fself.py Struct.py sfo.py pkg.py
