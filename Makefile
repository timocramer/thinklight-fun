CFLAGS=-O2 -Wall -Wextra -pipe
LDFLAGS=-L. -lthinklight

PREFIX=/usr/local
BINDIR=$(PREFIX)/bin

.PHONY: all clean install

all: tl-on tl-off tl-morse tl-strobo

tl-morse: tl-morsestrobo.c libthinklight.a
	$(CC) -o $@ $(CFLAGS) -DMORSE $< $(LDFLAGS)

tl-strobo: tl-morsestrobo.c libthinklight.a
	$(CC) -o $@ $(CFLAGS) -DSTROBO $< $(LDFLAGS)

tl-on: tl-onoff.c libthinklight.a
	$(CC) -o $@ $(CFLAGS) -DON $< $(LDFLAGS)

tl-off: tl-onoff.c libthinklight.a
	$(CC) -o $@ $(CFLAGS) -DOFF $< $(LDFLAGS)

%.o: %.c thinklight.h
	$(CC) -c -o $@ $(CFLAGS) $<

libthinklight.a: tl_init.o tl_deinit.o tl_on.o tl_off.o tl_status.o tl_toggle.o
	ar cr $@ $^
	ranlib $@

clean:
	$(RM) tl-on tl-off tl-morse tl-strobo *.o libthinklight.a

install:
	install -Ds -m u+rwx,go+rx,u+s tl-on tl-off tl-morse tl-strobo $(BINDIR)
