#include <errno.h>
#include <lmdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdint.h>

#include "macrologger.h"

#define LMDB_FAILURE(rc) \
            do {         \
            LOG_ERROR("%s", mdb_strerror(rc)); \
            exit(1);            \
            } while (0)

int main() {

  LOG_INFO("Started!");

  uint num_keys = 10000000;
  size_t key_size = 64;
  size_t val_size = 100;

  char *db_path = "lmdb";
  char *db_data_path = "lmdb/data.mdb";

  int rc;
  MDB_env *env;
  MDB_txn *txn;
  MDB_dbi dbi;
  int env_opt = 0 | MDB_NOSYNC | MDB_NOMETASYNC;
  size_t msize;
  unsigned int max_readers = 8;

  rc = mkdir(db_path, 0775);
  if (rc && errno != EEXIST) {
    perror("mkdir");
    exit(1);
  }

  rc = remove(db_data_path);
  if (rc && errno != ENOENT) {
    perror("remove");
    exit(1);
  }

  // environment options: MDB_NOSYNC, MDB_NOMETASYNC, MDB_WRITEMAP,
  // MDB_NORDAHEAD, MDB_CLEANMEM, MDB_DIRECTIO
  // msize = FLAGS_num*32L*FLAGS_value_size/10;
  // mdb_env_set_pagesize(env, FLAGS_pagesize);

  rc = mdb_env_create(&env);
  if (rc)
    LMDB_FAILURE(rc);

  msize = num_keys * 32L * (val_size + key_size) / 10;
  rc = mdb_env_set_mapsize(env, msize);
  if (rc)
    LMDB_FAILURE(rc);

  rc = mdb_env_set_maxreaders(env, max_readers + 2);
  if (rc)
    LMDB_FAILURE(rc);

  rc = mdb_env_open(env, db_path, env_opt, 0664);
  if (rc)
    LMDB_FAILURE(rc);

  rc = mdb_txn_begin(env, NULL, 0, &txn);
  if (rc)
    LMDB_FAILURE(rc);

  // Passing null to open will only create a single table in the environment
  rc = mdb_open(txn, NULL, 0, &dbi);
  if (rc)
    LMDB_FAILURE(rc);

  rc = mdb_txn_commit(txn);
  if (rc)
    LMDB_FAILURE(rc);

  MDB_val key;
  MDB_val data;

  char key_arr[key_size];
  char val_arr[val_size];
  key.mv_data = key_arr;
  key.mv_size = sizeof(key_arr);
  data.mv_data = val_arr;
  data.mv_size = sizeof(val_arr);

  for (uint64_t idx = 0; idx < num_keys; idx++) {

    uint64_t *ptr = key_arr;
    ptr[0] = idx;

    rc = mdb_txn_begin(env, NULL, 0, &txn);
    if (rc)
      LMDB_FAILURE(rc);

    rc = mdb_put(txn, dbi, &key, &data, 0);
    if (rc)
      LMDB_FAILURE(rc);

    rc = mdb_txn_commit(txn);
    if (rc)
      LMDB_FAILURE(rc);
  }

  LOG_INFO("Syncing Data!");

  rc = mdb_env_sync(env, 1);
  if (rc)
    LMDB_FAILURE(rc);

  LOG_INFO("Finished!");

  return 0;
}
