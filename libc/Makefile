BUILD_DIR = build/$(ARCH)

CFLAGS += -Wall -nostdinc -I../include -I. -I$(BUILD_DIR)/gen_src

OBJS = $(BUILD_DIR)/sys_fcntl.o \
	$(BUILD_DIR)/sys_mman.o \
	$(BUILD_DIR)/sys_poll.o \
	$(BUILD_DIR)/sys_resource.o \
	$(BUILD_DIR)/sys_signal.o \
	$(BUILD_DIR)/sys_socket.o \
	$(BUILD_DIR)/sys_stat.o \
	$(BUILD_DIR)/sys_statvfs.o \
	$(BUILD_DIR)/sys_stdio.o \
	$(BUILD_DIR)/sys_stdlib.o \
	$(BUILD_DIR)/sys_termios.o \
	$(BUILD_DIR)/sys_time.o \
	$(BUILD_DIR)/sys_times.o \
	$(BUILD_DIR)/sys_uio.o \
	$(BUILD_DIR)/sys_unistd.o \
	$(BUILD_DIR)/sys_utsname.o \
	$(BUILD_DIR)/sys_wait.o \
	$(BUILD_DIR)/atexit.o \
	$(BUILD_DIR)/bsearch.o \
	$(BUILD_DIR)/calloc.o \
	$(BUILD_DIR)/ctype.o \
	$(BUILD_DIR)/env.o \
	$(BUILD_DIR)/exit.o \
	$(BUILD_DIR)/fd_set.o \
	$(BUILD_DIR)/fopen.o \
	$(BUILD_DIR)/gets.o \
	$(BUILD_DIR)/init_stdio.o \
	$(BUILD_DIR)/qsort.o \
	$(BUILD_DIR)/realpath.o \
	$(BUILD_DIR)/setbuf.o \
	$(BUILD_DIR)/sigset.o \
	$(BUILD_DIR)/sprintf.o \
	$(BUILD_DIR)/stdio.o \
	$(BUILD_DIR)/stdio_priv.o \
	$(BUILD_DIR)/stdlib.o \
	$(BUILD_DIR)/strdup.o \
	$(BUILD_DIR)/string.o \
	$(BUILD_DIR)/strings.o \
	$(BUILD_DIR)/termios.o \
	$(BUILD_DIR)/ulltostr.o \
	$(BUILD_DIR)/wchar.o \
	$(BUILD_DIR)/wctype.o \
	$(BUILD_DIR)/c_ctype.o \
	$(BUILD_DIR)/w_ctype.o \
	$(BUILD_DIR)/c_fprintf.o \
	$(BUILD_DIR)/w_fprintf.o \
	$(BUILD_DIR)/c_snprintf.o \
	$(BUILD_DIR)/w_snprintf.o \
	$(BUILD_DIR)/c_stdio.o \
	$(BUILD_DIR)/w_stdio.o \
	$(BUILD_DIR)/c_stdlib.o \
	$(BUILD_DIR)/w_stdlib.o \
	$(BUILD_DIR)/c_string.o \
	$(BUILD_DIR)/w_string.o \
	$(BUILD_DIR)/c_vxprintf.o \
	$(BUILD_DIR)/w_vxprintf.o

LIB = $(BUILD_DIR)/libc.a

$(BUILD_DIR)/c_%.o: w_%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DW="'c'" -c -o $@ $<

$(BUILD_DIR)/w_%.o: w_%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DW="'w'" -c -o $@ $<

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

all: $(LIB)

$(BUILD_DIR)/.empty:
	mkdir -p $(BUILD_DIR)
	touch $(BUILD_DIR)/.empty

$(BUILD_DIR)/gen_src/.empty:
	mkdir -p $(BUILD_DIR)/gen_src
	touch $(BUILD_DIR)/gen_src/.empty

$(BUILD_DIR)/gen_src/float_priv.h: $(BUILD_DIR)/gen_src/.empty gen_float_priv_h.sh
	CC="$(CC)" CFLAGS="$(CFLAGS)" ./gen_float_priv_h.sh > $(BUILD_DIR)/gen_src/float_priv.h

$(LIB): $(OBJS)
	$(AR) -rc $(LIB) $(OBJS)
	$(AR) -ts $(LIB)

clean:
	rm -fr $(BUILD_DIR) *~

$(BUILD_DIR)/c_stdlib.o: w_stdlib.c $(BUILD_DIR)/gen_src/float_priv.h
$(BUILD_DIR)/w_stdlib.o: w_stdlib.c $(BUILD_DIR)/gen_src/float_priv.h
$(BUILD_DIR)/c_vxprintf.o: w_vxprintf.c $(BUILD_DIR)/gen_src/float_priv.h
$(BUILD_DIR)/w_vxprintf.o: w_vxprintf.c $(BUILD_DIR)/gen_src/float_priv.h
