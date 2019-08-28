IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=../obj

_DEPS = parse.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = dt.o parse.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

dt: $(OBJ)
	$(CC) -o ../bin/$@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
