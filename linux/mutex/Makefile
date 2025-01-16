CFLAGS= -O2

PROGS= mutex priority_inversion

all: $(PROGS)

.c:
	$(CC) -o $@ $< -lpthread 

clean: 
	rm -f *~ $(PROGS)
