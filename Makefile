CC ?= mpicc
CFLAGS ?= -g -O0
GSLIB ?= ../gslib.git/install

all: submit

submit: hang
	qsub ./s.bin

run: hang
	mpirun -np 12 ./hang ./con.co2

format:
	clang-format -i *.c

clean:
	$(RM) hang

% : %.c
	$(CC) $(CFLAGS) -I$(GSLIB)/include $< -o $@ -L$(GSLIB)/lib -lgs
