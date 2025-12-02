CC ?= mpicc
CFLAGS ?= -g -O0
GSLIB ?= ../gslib.git/install

all: submit

submit: hang
	qsub ./s.bin

run: hang
	mpirun -np 12 ./hang ./con.co2

clean:
	$(RM) hang

% : %.c
	$(CC) $(CFLAGS) -I$(GSLIB)/include $< -o $@ -L$(GSLIB)/lib -lgs
