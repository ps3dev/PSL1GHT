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

CFLAGS		:= $(DEBUGFLAGS) -O2 -Wall --std=gnu99
LDFLAGS		:= $(DEBUGFLAGS)

ifneq (,$(findstring MINGW,$(UNAME)))
	POSTFIX	:= .exe
	CFLAGS	+= -mno-cygwin
	LDFLAGS += -mno-cygwin -s -lelf
	OS		:= win32
endif

ifneq (,$(findstring CYGWIN,$(UNAME)))
	POSTFIX	:= .exe
	LDFLAGS += -s -lelf
	OS		:= win32
endif

ifneq (,$(findstring Darwin,$(UNAME)))
	CFLAGS	+= -I/opt/local/include
	LDFLAGS	+= -L/opt/local/lib -lelf
	OS		:= Mac
endif

ifneq (,$(findstring BSD,$(UNAME)))
	LDFLAGS += -lelf
endif

ifneq (,$(findstring Linux,$(UNAME)))
	LDFLAGS += -lelf
	OS		:= Linux
endif

TARGET		:= sprxlinker$(POSTFIX)
SOURCE		:= .
INCLUDE		:= .

CC			:= gcc

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

