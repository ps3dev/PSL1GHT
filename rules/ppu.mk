PREFIX		:= ppu-

include $(PSL1GHT)/host/base.mk

PSL1GHT_BIN	:= $(PSL1GHT)/host/bin

FSELF		:= $(PSL1GHT_BIN)/fself.py
SELF		:= $(FSELF)
#SELF_NPDRM	:= $(FSELF) -n
#SELF		:= $(PSL1GHT_BIN)/make_self
SELF_NPDRM	:= $(PSL1GHT_BIN)/make_self_npdrm
SFO			:= $(PSL1GHT_BIN)/sfo.py
PS3LOADAPP	:= $(PSL1GHT_BIN)/ps3load
PKG			:= $(PSL1GHT_BIN)/pkg.py
PKG_GEO		:= $(PSL1GHT_BIN)/package_finalize
SPRX		:= $(PSL1GHT_BIN)/sprxlinker
CGCOMP		:= $(PSL1GHT_BIN)/cgcomp

SFOXML		:= $(PSL1GHT_BIN)/sfo.xml
ICON0		:= $(PSL1GHT_BIN)/ICON0.PNG

CFLAGS		:= -g \
			   -I$(PSL1GHT)/target/include \
			   -I$(PS3DEV)/host/ppu/include
CXXFLAGS	:= $(CFLAGS)
LDFLAGS		:= -B$(PSL1GHT)/target/lib \
			   -B$(PS3DEV)/host/ppu/lib \
			   $(PS3DEV)/host/ppu/ppu/lib/lv2-psl1ght.o \
			   -lpsl1ght -llv2

%.self: %.elf
	@echo "[SELF] $(notdir $@)"
	@$(STRIP) $< -o $(BUILDDIR)/$(notdir $<)
	@$(SPRX) $(BUILDDIR)/$(notdir $<)
	@$(SELF) $(BUILDDIR)/$(notdir $<) $@

%.vcg.h: %.vcg
	@echo "[CGCOMP] $(notdir $<)"
	@$(CGCOMP) -v $< $(notdir $(BUILDDIR)/$(basename $<).rvp) >> /dev/null
	@$(RAW2H)  $(BUILDDIR)/$(notdir $(basename $<).rvp) $(BUILDDIR)/$(notdir $<).h $(BUILDDIR)/$(notdir $<).S $(notdir $(basename $<)_bin)
	@$(AS) -x assembler-with-cpp $(ASFLAGS) -c $(BUILDDIR)/$(notdir $<).S -o $(BUILDDIR)/$(notdir $<).o

%.fcg.h: %.fcg
	@echo "[CGCOMP] $(notdir $<)"
	@$(CGCOMP) -f $< $(notdir $(BUILDDIR)/$(basename $<).rvp) >> /dev/null
	@$(RAW2H)  $(BUILDDIR)/$(notdir $(basename $<).rvp) $(BUILDDIR)/$(notdir $<).h $(BUILDDIR)/$(notdir $<).S $(notdir $(basename $<)_bin)
	@$(AS) -x assembler-with-cpp $(ASFLAGS) -c $(BUILDDIR)/$(notdir $<).S -o $(BUILDDIR)/$(notdir $<).o

%.pkg: %.self
	@echo "[PKG] $(notdir $@)"
	@mkdir -p $(BUILDDIR)/pkg
	@mkdir -p $(BUILDDIR)/pkg/USRDIR
	@cp $(ICON0) $(BUILDDIR)/pkg/
	@$(SELF_NPDRM) $(BUILDDIR)/$(basename $(notdir $<)).elf $(BUILDDIR)/pkg/USRDIR/EBOOT.BIN $(CONTENTID) >> /dev/null
	@$(SFO) --title "$(TITLE)" --appid "$(APPID)" -f $(SFOXML) $(BUILDDIR)/pkg/PARAM.SFO
	@if test -n "$(PKGFILES)" -a -d "$(PKGFILES)"; then cp -rf $(PKGFILES)/* $(BUILDDIR)/pkg/; fi
	@$(PKG) --contentid $(CONTENTID) $(BUILDDIR)/pkg/ $@ >> /dev/null
	@echo "[PKG] $(basename $(notdir $@)).geohot.pkg"
	@cp $@ $(basename $@).geohot.pkg
	@$(PKG_GEO) $(basename $@).geohot.pkg
