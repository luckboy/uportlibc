CFLAGS += -Wall -nostdinc -I../include

OBJS = string.o cstring.o wstring.o

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

c%.o: w_%.c
	$(CC) $(CFLAGS) -D__W="'c'" -c -o $@ $<

w%.o: w_%.c
	$(CC) $(CFLAGS) -D__W="'w'" -c -o $@ $<

all: libc.a

libc.a: $(OBJS)
	$(AR) r libc.a $(OBJS)

clean:
	rm -fr libc.a $(OBJS) *~
