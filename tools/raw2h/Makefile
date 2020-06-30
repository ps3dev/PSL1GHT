#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(PSL1GHT)),)
$(error "Please set PSL1GHT in your environment. export PSL1GHT=<path>")
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
	@[ -d $(PSL1GHT)/bin ] || mkdir -p $(PSL1GHT)/bin
	@cp $(TARGET) $(PSL1GHT)/bin/

$(TARGET): $(CFILES)
	@$(CC) $(CFLAGS) $< -o $@
