all:
	for d in libc; do $(MAKE) -C $$d all; done

clean:
	for d in libc; do $(MAKE) -C $$d clean; done
