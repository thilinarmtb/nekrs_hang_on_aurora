CC ?= mpicc
CFLAGS ?= -g -O0
GSLIB ?= ../gslib.git/install

all: run

clean:
	$(RM) hang

run: hang
	mpirun -np 12 ./hang ./con.co2

% : %.c
	$(CC) $(CFLAGS) -I$(GSLIB)/include $< -o $@ -L$(GSLIB)/lib -lgs
