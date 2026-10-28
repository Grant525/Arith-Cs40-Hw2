CC = gcc

IFLAGS = -I/comp/40/build/include -I/usr/sup/cii40/include/cii

CFLAGS = -g -std=gnu99 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64

LDLIBS = -l40locality -larith40 -lnetpbm -lcii40 -lm -lrt

INCLUDES = $(shell echo *.h)

all: 40image


%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@


40image: 40image.o compress40.o images.o conversions.o block.o codeword.o bitpack.o \
         a2plain.o a2blocked.o uarray2.o uarray2b.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

ppmdiff: ppmdiff.o a2plain.o uarray2.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

a2test: a2test.o uarray2b.o uarray2.o a2plain.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

timing_test: timing_test.o cputiming.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS) 

ppmtrans: ppmtrans.o cputiming.o a2plain.o a2blocked.o uarray2b.o uarray2.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f 40image ppmdiff ppmtrans a2test timing_test *.o
