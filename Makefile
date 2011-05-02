#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(PSL1GHT)),)
$(error "Please set PSL1GHT in your environment. export PSL1GHT=<path>")
endif

.PHONY: samples

all:
	@$(MAKE) -C common --no-print-directory
	@$(MAKE) -C ppu --no-print-directory
	@$(MAKE) -C spu --no-print-directory
	@$(MAKE) -C tools --no-print-directory

samples:
	@$(MAKE) -C samples --no-print-directory

doc:
	@doxygen doxygen.conf

install-ctrl:
	@[ -d $(PSL1GHT) ] || mkdir -p $(PSL1GHT)
	@cp -frv base_rules $(PSL1GHT)
	@cp -frv ppu_rules  $(PSL1GHT)
	@cp -frv spu_rules  $(PSL1GHT)

install-socat:
	@$(MAKE) -C tools install-socat --no-print-directory

install:
	@$(MAKE) -C common install --no-print-directory
	@$(MAKE) -C ppu install --no-print-directory
	@$(MAKE) -C spu install --no-print-directory
	@$(MAKE) -C tools install --no-print-directory

clean:
	@$(MAKE) -C common clean --no-print-directory
	@$(MAKE) -C ppu clean --no-print-directory
	@$(MAKE) -C spu clean --no-print-directory
	@$(MAKE) -C tools clean --no-print-directory
	@rm -rf doc

.PHONY: all clean install
