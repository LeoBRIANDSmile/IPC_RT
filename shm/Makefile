CFLAGS= -O2

PROGS= shm

all: $(PROGS)

.c:
	$(CC) -o $@ $< -lpthread 

clean: 
	rm -f *~ $(PROGS)
