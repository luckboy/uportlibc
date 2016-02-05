DIRS = libc

all:
	@arch="$(ARCH)"; \
	if [ "$$arch" = "" ]; then arch=`$(CC) -dumpmachine | cut -f 1 -d -`; fi; \
	for d in $(DIRS); do $(MAKE) -C $$d all ARCH="$$arch"; done

clean:
	@arch="$(ARCH)"; \
	if [ "$$arch" = "" ]; then arch=`$(CC) -dumpmachine | cut -f 1 -d -`; fi; \
	for d in $(DIRS); do $(MAKE) -C $$d clean ARCH="$$arch"; done
