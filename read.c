#include <stdio.h>
#include <stdlib.h>
#include <gslib.h>

static int setup_gs(slong *ids, size_t size1, size_t size2,
    const struct comm *c) {
  struct gs_data *gsh = gs_setup(ids, size1 + size2, c, 0, gs_pairwise, 0);
  gs_free(gsh);
  return 0;
}

static int read_ids(slong **ids, size_t *s1, size_t *s2, const char *fname,
    const struct comm *c) {
  MPI_File file;
  int err = MPI_File_open(c->c, fname, MPI_MODE_RDONLY, MPI_INFO_NULL, &file);
  if (err) goto print_error_and_exit;

  // Header size:
  size_t hsize = 2;
  // Rank 0 reads the number of MPI processes in addition to size1 and size2.
  if (c->id == 0) hsize++;
  size_t rsize = hsize * sizeof(size_t);

  slong out[2][1], wrk[2][1], in = rsize;
  comm_scan(out, c, gs_long, gs_add, &in, 1, wrk);
  MPI_Offset off = (MPI_Offset)out[0][0], hoff = (MPI_Offset)out[1][0];

  // Read the meta info:
  size_t header[3];
  MPI_Status st;
  MPI_File_read_at(file, off, (void *)header, rsize, MPI_BYTE, &st);

  // Some sanity checks:
  if (c->id == 0) {
    assert(header[0] == (size_t)c->np && "read and write sizes don't march!");
    printf("np = %zu size1=%zu size2=%zu\n", header[0], header[1], header[2]);
  }
  fflush(stdout);

  // Write the ids
  size_t size1 = *s1 = header[1 - (c->id > 0)];
  size_t size2 = *s2 = header[2 - (c->id > 0)];
  rsize = sizeof(slong) * (size1 + size2);

  in = rsize;
  comm_scan(out, c, gs_long, gs_add, &in, 1, wrk);
  off = hoff + (MPI_Offset)out[0][0];

  *ids = calloc(rsize, sizeof(char));
  MPI_File_read_at(file, off, (void *)(*ids), rsize, MPI_BYTE, &st);

  return 0;

print_error_and_exit:
  if (c->id == 0)
    fprintf(stderr, "Error opening file: %s for writing\n", fname);
  fflush(stderr);

  *ids = 0, *s1 = *s2 = 0;

  return 1;
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [co2 file name]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct comm c;
  comm_init(&c, MPI_COMM_WORLD);

  slong *ids = 0;
  size_t size1, size2;
  read_ids(&ids, &size1, &size2, argv[1], &c);

  setup_gs(ids, size1, size2, &c);

  free(ids);
  comm_free(&c);
  MPI_Finalize();

  return 0;
}
