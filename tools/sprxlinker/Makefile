#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

UNAME	:=	$(shell uname -s)

ifeq ($(strip $(PS3DEV)),)
  ifeq ($(strip $(DEVKITPS3)),)
    export PS3DEV := /usr/local/ps3dev
  else
    export PS3DEV := $(DEVKITPS3)
  endif
endif

ifneq (,$(findstring MINGW,$(UNAME)))
	POSTFIX	:= .exe
endif

ifneq (,$(findstring CYGWIN,$(UNAME)))
	POSTFIX	:= .exe
endif

TARGET		:= sprxlinker$(POSTFIX)
SOURCE		:= .
INCLUDE		:= .

CFLAGS		:= -O2 -Wall --std=gnu99 -I/opt/local/include
LDFLAGS		:= -L/opt/local/lib -lelf

CC		:= gcc

CFILES		:= $(foreach dir,$(SOURCE),$(wildcard $(dir)/*.c))
INCLUDES	:= $(foreach dir,$(INCLUDE),-I$(dir))

all: $(TARGET)

install: all
	@echo Installing $(TARGET)
	@[ -d $(PS3DEV)/bin ] || mkdir -p $(PS3DEV)/bin
	@install -m 755 $(TARGET) $(PS3DEV)/bin

clean:
	@echo clean ...
	@rm -rf $(TARGET)

$(TARGET): $(CFILES)
			@echo building ... $(notdir $@)
			@$(CC) $(CFLAGS) $(INCLUDES) $< -o $@ $(LDFLAGS)

