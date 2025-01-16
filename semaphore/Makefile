CFLAGS= -O2

PROGS= semaphore

all: $(PROGS)

.c:
	$(CC) -o $@ $< -lpthread 

clean: 
	rm -f *~ $(PROGS)
