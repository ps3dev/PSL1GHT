TARGETS := debugtest echoserv httptest networktest ps3load

all:
	@for TARGET in $(TARGETS); do $(MAKE) --no-print-directory -C $$TARGET; done

clean:
	@for TARGET in $(TARGETS); do $(MAKE) --no-print-directory -C $$TARGET clean; done
pkg:
	@for TARGET in $(TARGETS); do $(MAKE) --no-print-directory -C $$TARGET pkg; done
