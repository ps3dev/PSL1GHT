TARGETS := spuchain spudma sputest sputhread spuparallel

all:
	@for TARGET in $(TARGETS); do $(MAKE) --no-print-directory -C $$TARGET bin; done
	@for TARGET in $(TARGETS); do $(MAKE) --no-print-directory -C $$TARGET; done

clean:
	@for TARGET in $(TARGETS); do $(MAKE) --no-print-directory -C $$TARGET clean; done
