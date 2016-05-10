CC     = gcc
CFLAGS = -g -I. -std=c99 -Wall -D__USE_FIXED_PROTOTYPES__
PROG   = split_string

$(PROG): $(PROG).o
		$(CC) $(CFLAGS) -o $@ $^

%.o: %.c %.h
		$(CC) $(CFLAGS) -c -o $@ $<

clean:
		$(RM) $(PROG) *.o



