CFLAGS += -Wall -nostdinc -I../include

OBJS = ctype.o string.o c_string.o w_string.o c_ctype.o w_ctype.o

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

c_%.o: w_%.c
	$(CC) $(CFLAGS) -D__W="'c'" -c -o $@ $<

w_%.o: w_%.c
	$(CC) $(CFLAGS) -D__W="'w'" -c -o $@ $<

all: libc.a

libc.a: $(OBJS)
	$(AR) r libc.a $(OBJS)

clean:
	rm -fr libc.a $(OBJS) *~
