#include <errno.h>
#include <lmdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void lmdb_failure(int rc, char *msg) {
  fprintf(stderr, "%s: %s\n", msg, mdb_strerror(rc));
  exit(1);
}
int main() {

  uint num_keys = 1000;
  size_t key_size = 100;
  size_t val_size = 100;

  char *db_path = "/tmp/lmdb";

  int rc;
  MDB_env *env;
  MDB_txn *txn;
  MDB_dbi dbi;
  int env_opt = 0;
  char file_name[100];
  size_t msize;
  unsigned int max_readers = 8;

  rc = mkdir(db_path, 0775);
  if (rc && errno != EEXIST) {
    perror("mkdir");
    exit(1);
  }

  // environment options: MDB_NOSYNC, MDB_NOMETASYNC, MDB_WRITEMAP,
  // MDB_NORDAHEAD, MDB_CLEANMEM, MDB_DIRECTIO
  // msize = FLAGS_num*32L*FLAGS_value_size/10;
  // mdb_env_set_pagesize(env, FLAGS_pagesize);

  rc = mdb_env_create(&env);
  if (rc)
    lmdb_failure(rc, "create environment error");

  msize = num_keys * 32L * val_size / 10;
  rc = mdb_env_set_mapsize(env, msize);
  if (rc)
    lmdb_failure(rc, "set size error");

  rc = mdb_env_set_maxreaders(env, max_readers + 2);
  if (rc)
    lmdb_failure(rc, "set max readers");

  rc = mdb_env_open(env, db_path, env_opt, 0664);
  if (rc)
    lmdb_failure(rc, "open error");

  rc = mdb_txn_begin(env, NULL, 0, &txn);
  if (rc)
    lmdb_failure(rc, "txn begin error");

  // Passing null to open will only create a single table in the environment
  rc = mdb_open(txn, NULL, 0, &dbi);
  if (rc)
    lmdb_failure(rc, "mdb open error");

  rc = mdb_txn_commit(txn);
  if (rc)
    lmdb_failure(rc, "txn commit error");

  printf("Hello, World!\n");
  return 0;
}
