CFLAGS= -O2

PROGS= message_queue

all: $(PROGS)

.c:
	$(CC) -o $@ $< -lpthread 

clean: 
	rm -f *~ $(PROGS)
