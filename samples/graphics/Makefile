TARGETS := rsxtest rsxtest_spu cairo # jpgtest pngtest rsxTest rsxTestVP videoTest

all:
	@for TARGET in $(TARGETS); do $(MAKE) --no-print-directory -C $$TARGET; done

clean:
	@for TARGET in $(TARGETS); do $(MAKE) --no-print-directory -C $$TARGET clean; done
