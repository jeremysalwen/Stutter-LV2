CFLAGS += -O3 -g -Wall -ftree-vectorizer-verbose=5  -msse2 -msse -msse3 -march=native -mtune=native --std=c99 `pkg-config --cflags --libs lv2core` -fPIC -ffast-math
OBJECTS = stutterlv2.o
LIBRARY = libstutterlv2.so

TTLS = stutter.ttl manifest.ttl
INSTALLDIR = $(DESTDIR)/usr/lib/lv2/
INSTALLNAME = stutter.lv2/

all: libstutterlv2.so
$(LIBRARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -shared -o $@

.SUFFIXES:
.SUFFIXES:	.c .C .o

.c.o :
	$(CC) -o $@ -c $(CFLAGS) $< $(INCFLAGS)

count:
	wc *.c *.cc *.C *.cpp *.h *.hpp

clean:
	rm -f $(LIBRARY) $(OBJECTS)

install :
	mkdir -p $(INSTALLDIR)$(INSTALLNAME)
	install $(LIBRARY) $(INSTALLDIR)$(INSTALLNAME)
	install $(TTLS) $(INSTALLDIR)$(INSTALLNAME)

uninstall :
	rm -rf $(INSTALLDIR)$(INSTALLNAME)

.PHONY: all install clean uninstall
