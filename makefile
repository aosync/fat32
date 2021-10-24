PROG = fatstat
OBJ = fatstat.o fat32.o

.SUFFIXES: .c .o

.c.o:
	$(CC) -c $< $(CFLAGS)

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

.PHONY: clean

clean:
	rm $(PROG) $(OBJ)
