#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

all:
	@$(MAKE) -C crt --no-print-directory
	@$(MAKE) -C sprx --no-print-directory
	@$(MAKE) -C librt --no-print-directory
	@$(MAKE) -C librsx --no-print-directory

install-headers:
	@[ -d $(DESTDIR)$(PSL1GHT)/ppu ] || mkdir -p $(DESTDIR)$(PSL1GHT)/ppu
	@cp -frv include $(DESTDIR)$(PSL1GHT)/ppu

install: install-headers
	@$(MAKE) -C crt install --no-print-directory
	@$(MAKE) -C sprx install --no-print-directory
	@$(MAKE) -C librt install --no-print-directory
	@$(MAKE) -C librsx install --no-print-directory

clean:
	@$(MAKE) -C crt clean --no-print-directory
	@$(MAKE) -C sprx clean --no-print-directory
	@$(MAKE) -C librt clean --no-print-directory
	@$(MAKE) -C librsx clean --no-print-directory

.PHONY: all clean install
