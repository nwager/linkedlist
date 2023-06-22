CC:=gcc
IDIR:=include
SDIR:=src
ODIR:=obj
OUTDIR:=out
TDIR:=test
CFLAGS:=-Wall -Wpedantic -Wextra -Werror -g -I$(IDIR)
LIBOUT:=libout

_DEPS:=linkedlist.h
DEPS:=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJS:=linkedlist.o
OBJS:=$(patsubst %,$(ODIR)/%,$(_OBJS))

_OUT:=liblinkedlist.a
OUTPUT:=$(OUTDIR)/$(_OUT)

TOUTPUT:=$(OUTDIR)/test_linkedlist.out

# targets

all: $(OUTPUT)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUTPUT): $(OBJS)
	mkdir -p $(OUTDIR)
	ar cr $(OUTPUT) $^

test: $(OUTPUT) $(TDIR)/test_linkedlist.c
	$(CC) -o $(TOUTPUT) $^ $(CFLAGS)

runtest: test
	# Running tests
	./$(TOUTPUT)
	# Tests passed

runleak: test
	# Running tests
	leaks --atExit -- $(TOUTPUT)
	# Tests passed

lib: $(OUTPUT)
	mkdir -p $(LIBOUT)
	cp $(OUTPUT) $(DEPS) $(LIBOUT)

.PHONY: clean

clean:
	rm -rf *.o *.a *~ $(ODIR) $(OUTDIR) $(TOUTPUT) $(OUTDIR) $(OUTDIR) $(LIBOUT)