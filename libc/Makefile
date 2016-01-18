CFLAGS += -Wall -nostdinc -I../include

OBJS = sys_fcntl.o sys_mman.o sys_poll.o sys_resource.o sys_signal.o sys_socket.o sys_stat.o sys_statvfs.o \
	sys_termios.o sys_time.o sys_times.o sys_uio.o sys_unistd.o sys_utsname.o sys_wait.o \
	ctype.o string.o wctype.o \
	c_ctype.o w_ctype.o c_string.o w_string.o

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

c_%.o: w_%.c
	$(CC) $(CFLAGS) -DW="'c'" -c -o $@ $<

w_%.o: w_%.c
	$(CC) $(CFLAGS) -DW="'w'" -c -o $@ $<

all: libc.a

libc.a: $(OBJS)
	$(AR) r libc.a $(OBJS)

clean:
	rm -fr libc.a $(OBJS) *~
