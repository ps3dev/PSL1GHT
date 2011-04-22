##
## Configuration
##

PREFIX          := ppu

TARGET		:= audiosync.self

TITLE		:= Template - PSL1GHT
APPID		:= TEST00003
CONTENTID	:= UP0001-$(APPID)_00-0000000000000000
ICON0		:= /usr/local/ps3dev/bin/ICON0.PNG
SFOXML		:= /usr/local/ps3dev/bin/sfo.xml

FLAGS		:= -std=gnu99
INCLUDES	:=
LIBS		:= -laudio -lrt -llv2 -lm

##
## Tools
##

AS		:= $(PREFIX)-as
CC		:= $(PREFIX)-gcc
CPP		:= $(PREFIX)-g++
LD		:= $(PREFIX)-g++

ASFLAGS		:=
CFLAGS		:= -Wall -pedantic -O3 $(FLAGS) -I$(PSL1GHT)/$(PREFIX)/include $(INCLUDES)
CPPFLAGS	:= $(CFLAGS)
LDFLAGS		:= -L$(PSL1GHT)/$(PREFIX)/lib $(LIBS)

MAKE_SELF	:= make_self_npdrm
MAKE_SFO	:= sfo.py
MAKE_PKG	:= pkg.py

##
## Files
##

RAWFILES	:= $(shell find * -name "*.raw" -print )
CFILES		:= $(shell find * -name "*.c"   -print )
CPPFILES	:= $(shell find * -name "*.cpp" -print )
SFILES		:= $(shell find * -name "*.s"   -print )

OFILES		:= ${RAWFILES:.raw=.o} ${CFILES:.c=.o} ${CPPFILES:.cpp=.o} ${SFILES:.s=.o}

##
## Rules
##

default: $(TARGET)

clean:
	rm -f $(OFILES) $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

%.o: %.raw
	@bin2s -a 64 $< | $(AS) $(ASFLAGS) -o $@
	@echo "extern const u8"  `(echo $(<F) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"_end[];" >  `(echo $< | tr . _)`.h
	@echo "extern const u8"  `(echo $(<F) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"[];"     >> `(echo $< | tr . _)`.h
	@echo "extern const u32" `(echo $(<F) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`_size";"  >> `(echo $< | tr . _)`.h

%.o: %.s
	$(AS) $(ASFLAGS) -o $@

%.a: $(OFILES)
	$(AR) cru $@ $(OFILES)

%.elf: $(OFILES)
	$(LD) $(OFILES) $(LDFLAGS) -o $@

%.self: %.elf
	$(MAKE_SELF) $< $@ $(CONTENTID) >> /dev/null

%.pkg: %.elf
	@rm -Rf pkg
	@mkdir -p pkg
	@mkdir -p pkg/USRDIR
	@cp $(ICON0) pkg
	$(MAKE_SELF) $< pkg/USRDIR/EBOOT.BIN $(CONTENTID) >> /dev/null
	$(MAKE_SFO) --title "$(TITLE)" --appid "$(APPID)" -f $(SFOXML) pkg/PARAM.SFO
	$(MAKE_PKG) --contentid $(CONTENTID) pkg/ $@ >> /dev/null
	@rm -Rf pkg
