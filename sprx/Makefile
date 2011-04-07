TARGETS	:=	libsysfs libaudio libcamera libgcm_sys libio libjpgdec liblv2 libnet libnetctl libpngdec libsysmodule libsysutil libgem libvdec

all:
	@for dir in $(TARGETS); do \
		echo; \
		echo Entering Directory $$dir; \
		$(MAKE) --no-print-directory -C $$dir; \
		echo Leaving Directory $$dir; \
	done

clean:
	@for dir in $(TARGETS); do \
		echo Cleaning $$dir; \
		$(MAKE) --no-print-directory -C $$dir clean; \
	done

install: all
	@mkdir -p $(PSL1GHT)/target/lib
	@cp */*.a $(PSL1GHT)/target/lib/

.PHONY: all install clean
