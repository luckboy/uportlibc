BUILD_DIR = build/$(ARCH)

CFLAGS = -g -DTEST=1 -I.
LDFLAGS = -g
LIBS = -lcunit

OBJS = $(BUILD_DIR)/main.o \
	$(BUILD_DIR)/c_string.o \
  	$(BUILD_DIR)/c_string_suite.o \
  	$(BUILD_DIR)/w_string.o \
  	$(BUILD_DIR)/w_string_suite.o

TEST = build/$(ARCH)/test

$(BUILD_DIR)/c_%.o: ../../libc/w_%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DW="'c'" -c -o $@ $<

$(BUILD_DIR)/w_%.o: ../../libc/w_%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DW="'w'" -c -o $@ $<

$(BUILD_DIR)/%.o: ../../libc/%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/c_%.o: w_%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DW="'c'" -c -o $@ $<

$(BUILD_DIR)/w_%.o: w_%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DW="'w'" -c -o $@ $<

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

all: $(TEST)

$(BUILD_DIR)/.empty:
	mkdir -p $(BUILD_DIR)
	touch $(BUILD_DIR)/.empty

$(TEST): $(OBJS)
	$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $(TEST) $(OBJS) $(LIBS) $(EXTRA_LIBS)

run: all
	./$(TEST)

clean:
	rm -fr rm -fr $(BUILD_DIR) *~
