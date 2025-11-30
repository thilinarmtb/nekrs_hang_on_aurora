CC ?= mpicc
CFLAGS ?= -g -O0
GSLIB ?= ../gslib.git/install

all: read

clean:
	$(RM) read

run: read
	mpirun -np 12 ./read ./con.co2

% : %.c
	$(CC) $(CFLAGS) -I$(GSLIB)/include $< -o $@ -L$(GSLIB)/lib -lgs
