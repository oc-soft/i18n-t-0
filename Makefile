

CD=cd

all: build-program


build-program:
	$(MAKE) -C src  all

clean: clean-program

clean-program:
	$(MAKE) -C src clean


.PHONY: clean-program


# vi: se ts=4 sw=4 noet:
