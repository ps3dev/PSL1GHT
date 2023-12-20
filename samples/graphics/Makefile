TARGETS := blitting cairo rsxtest rsxtest_dl rsxtest_flip rsxtest_spu rsxtest_tile videoTest

all:
	@for TARGET in $(TARGETS); do $(MAKE) --no-print-directory -C $$TARGET; done

clean:
	@for TARGET in $(TARGETS); do $(MAKE) --no-print-directory -C $$TARGET clean; done
pkg:
	@for TARGET in $(TARGETS); do $(MAKE) --no-print-directory -C $$TARGET pkg; done
