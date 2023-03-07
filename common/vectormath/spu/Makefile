#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

all:

install:
	@[ -d $(DESTDIR)$(PSL1GHT)/spu/include/vectormath ] || mkdir -p $(DESTDIR)$(PSL1GHT)/spu/include/vectormath
	@cp -frv $(CURDIR)/c $(DESTDIR)$(PSL1GHT)/spu/include/vectormath
	@cp -frv $(CURDIR)/cpp $(DESTDIR)$(PSL1GHT)/spu/include/vectormath

clean:

.PHONY: all clean install
