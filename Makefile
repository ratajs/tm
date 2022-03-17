PREFIX	= $(HOME)
BINDIR	= $(PREFIX)/bin
MANDIR	= $(PREFIX)/man/man1
CFLAGS	= -Wall -pedantic

all: tm

tm: tm.c tm.h

test: tm
	./tm add.tm   < add.in   | diff - add.out
	./tm max.tm   < max.in   | diff - max.out
	./tm sign.tm  < sign.in  | diff - sign.out
	./tm succ.tm  < succ.in  | diff - succ.out
	./tm proj.tm  < proj.in  | diff - proj.out
	./tm copy.tm  < copy.in  | diff - copy.out
	./tm mult.tm  < mult.in  | diff - mult.out
	./tm empty.tm < empty.in | diff - empty.out
	./tm table.tm < table.in | diff - table.out
	./tm bb1.tm   < bb1.in   | diff - bb1.out
	./tm bb2.tm   < bb2.in   | diff - bb2.out

lint: tm.1
	mandoc -Tlint -Wstyle tm.1

install: test tm.1
	install -d -m 755 $(BINDIR) && install -m 755 tm   $(BINDIR)
	install -d -m 755 $(MANDIR) && install -m 755 tm.1 $(MANDIR)

uninstall:
	rm -f $(BINDIR)/tm
	rm -f $(MANDIR)/tm.1

clean:
	rm -f tm *.o *.core *~
