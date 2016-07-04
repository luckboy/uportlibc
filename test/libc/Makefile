BUILD_DIR = build/$(ARCH)

CFLAGS = -g -DTEST=1 -I. -I$(BUILD_DIR)/gen_src -I../../libc
LDFLAGS = -g
LIBS = -lm -lcunit

OBJS = $(BUILD_DIR)/main.o \
	$(BUILD_DIR)/conv.o \
	$(BUILD_DIR)/bsearch.o \
  	$(BUILD_DIR)/bsearch_suite.o \
	$(BUILD_DIR)/qsort.o \
  	$(BUILD_DIR)/qsort_suite.o \
	$(BUILD_DIR)/c_stdlib.o \
  	$(BUILD_DIR)/c_stdlib_suite.o \
  	$(BUILD_DIR)/w_stdlib.o \
  	$(BUILD_DIR)/w_stdlib_suite.o \
	$(BUILD_DIR)/c_string.o \
  	$(BUILD_DIR)/c_string_suite.o \
  	$(BUILD_DIR)/w_string.o \
  	$(BUILD_DIR)/w_string_suite.o \
  	$(BUILD_DIR)/wchar.o \
  	$(BUILD_DIR)/wchar_suite.o

TEST = build/$(ARCH)/test

$(BUILD_DIR)/c_%.o: ../../libc/w_%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DW="'c'" -c -o $@ $<

$(BUILD_DIR)/w_%.o: ../../libc/w_%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -DW="'w'" -c -o $@ $<

$(BUILD_DIR)/%.o: ../../libc/%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/c_%.o: w_%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -std=c99 -DW="'c'" -c -o $@ $<

$(BUILD_DIR)/w_%.o: w_%.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -std=c99 -DW="'w'" -c -o $@ $<

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)/.empty
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -std=c99 -c -o $@ $<

all: $(TEST)

$(BUILD_DIR)/.empty:
	mkdir -p $(BUILD_DIR)
	touch $(BUILD_DIR)/.empty

$(BUILD_DIR)/gen_src/.empty:
	mkdir -p $(BUILD_DIR)/gen_src
	touch $(BUILD_DIR)/gen_src/.empty

$(BUILD_DIR)/gen_src/float_priv.h: $(BUILD_DIR)/gen_src/.empty ../../libc/gen_float_priv_h.sh
	CC="$(CC)" CFLAGS="$(CFLAGS)" ../../libc/gen_float_priv_h.sh > $(BUILD_DIR)/gen_src/float_priv.h	

$(TEST): $(OBJS)
	$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $(TEST) $(OBJS) $(LIBS) $(EXTRA_LIBS)

run: all
	./$(TEST)

clean:
	rm -fr rm -fr $(BUILD_DIR) *~

$(BUILD_DIR)/conv.o: ../../libc/conv.c $(BUILD_DIR)/gen_src/float_priv.h
$(BUILD_DIR)/c_stdlib.o: ../../libc/w_stdlib.c $(BUILD_DIR)/gen_src/float_priv.h
$(BUILD_DIR)/w_stdlib.o: ../../libc/w_stdlib.c $(BUILD_DIR)/gen_src/float_priv.h
