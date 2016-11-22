TARGETS := setjmp0 setjmp1 setjmp2 setjmp3

all: $(TARGETS)

clean:
	rm $(TARGETS)

setjmp0: setjmp.c
	gcc $< -o $@

setjmp1: setjmp.c
	gcc -O1 $< -o $@

setjmp2: setjmp.c
	gcc -O2 $< -o $@

setjmp3: setjmp.c
	gcc -O3 $< -o $@
