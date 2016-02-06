DIRS = libc

all clean:
	@arch="$(ARCH)"; \
	if [ "$$arch" = "" ]; then arch="`$(CC) -dumpmachine | cut -f 1 -d -`"; fi; \
	for d in $(DIRS); do $(MAKE) -C $$d $@ ARCH="$$arch"; done
