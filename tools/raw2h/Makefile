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
	@cp $(TARGET) $(PSL1GHT)/bin/

$(TARGET): $(CFILES)
	@$(CC) $(CFLAGS) $< -o $@
