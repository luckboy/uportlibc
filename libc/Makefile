BUILD_DIR = build/$(ARCH)

CFLAGS += -Wall -nostdinc -I../include -I.

OBJS = $(BUILD_DIR)/sys_fcntl.o \
	$(BUILD_DIR)/sys_mman.o \
	$(BUILD_DIR)/sys_poll.o \
	$(BUILD_DIR)/sys_resource.o \
	$(BUILD_DIR)/sys_signal.o \
	$(BUILD_DIR)/sys_socket.o \
	$(BUILD_DIR)/sys_stat.o \
	$(BUILD_DIR)/sys_statvfs.o \
	$(BUILD_DIR)/sys_termios.o \
	$(BUILD_DIR)/sys_time.o \
	$(BUILD_DIR)/sys_times.o \
	$(BUILD_DIR)/sys_uio.o \
	$(BUILD_DIR)/sys_unistd.o \
	$(BUILD_DIR)/sys_utsname.o \
	$(BUILD_DIR)/sys_wait.o \
	$(BUILD_DIR)/ctype.o \
	$(BUILD_DIR)/fd_set.o \
	$(BUILD_DIR)/sigset.o \
	$(BUILD_DIR)/string.o \
	$(BUILD_DIR)/wctype.o \
	$(BUILD_DIR)/c_ctype.o \
	$(BUILD_DIR)/w_ctype.o \
	$(BUILD_DIR)/c_string.o \
	$(BUILD_DIR)/w_string.o

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

$(LIB): $(OBJS)
	$(AR) -rc $(LIB) $(OBJS)
	$(AR) -ts $(LIB)

clean:
	rm -fr $(BUILD_DIR) *~
