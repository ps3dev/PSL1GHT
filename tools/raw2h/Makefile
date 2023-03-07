#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(PS3DEV)),)
$(error "Please set PS3DEV in your environment. export PS3DEV=<path>")
endif

TARGET		:= raw2h
SOURCE		:= .

CFLAGS		:= -O2 -Wall --std=gnu99

CC		:= gcc

CFILES		:= $(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.c)))

all: $(TARGET)

clean:
	@echo clean...
	@rm -rf $(TARGET) *.exe

install: all
	@echo Installing $(TARGET)
	@[ -d $(DESTDIR)$(PS3DEV)/bin ] || mkdir -p $(DESTDIR)$(PS3DEV)/bin
	@cp $(TARGET) $(DESTDIR)$(PS3DEV)/bin/

$(TARGET): $(CFILES)
	@$(CC) $(CFLAGS) $< -o $@
