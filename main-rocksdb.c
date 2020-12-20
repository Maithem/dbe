#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include "macrologger.h"

#include <rocksdb/c.h>
#include <unistd.h>

int main(int argc, char **argv) {
    rocksdb_t *db;
    rocksdb_options_t *options = rocksdb_options_create();

    long cpus = sysconf(_SC_NPROCESSORS_ONLN);

    // Set # of online cores
    rocksdb_options_increase_parallelism(options, (int) (cpus));
    rocksdb_options_optimize_level_style_compaction(options, 0);
    // create the DB if it's not already present
    rocksdb_options_set_create_if_missing(options, 1);
    rocksdb_options_set_max_open_files(options, 200);
    rocksdb_options_set_target_file_size_base(options, 100000000);
    rocksdb_options_set_target_file_size_multiplier(options, 10000000);

    // open DB
    char *err = NULL;
    char *DBPath = "rocksdb";
    db = rocksdb_open(options, DBPath, &err);
    assert(!err);


    uint num_keys = 10000000;
    size_t key_size = 64;
    size_t val_size = 100;
    char key_arr[key_size];
    char val_arr[val_size];

    rocksdb_writeoptions_t *writeoptions = rocksdb_writeoptions_create();
    LOG_INFO("Started!");
    for (uint64_t idx = 0; idx < num_keys; idx++) {
        uint64_t *ptr = key_arr;
        ptr[0] = idx;
        rocksdb_put(db, writeoptions, key_arr, sizeof(key_arr), val_arr, sizeof(val_arr),&err);
        if (err != NULL) {
            LOG_INFO("%s", err);
        }
        assert(!err);

        /**
        if (idx % 50000 == 0) {
            LOG_INFO("%" PRIu64 "\n", idx);
        }
         **/

    }

    LOG_INFO("Syncing Data!");
    rocksdb_flushoptions_t *flushoptions = rocksdb_flushoptions_create();
    rocksdb_flush(db, flushoptions, &err);
    assert(!err);
    LOG_INFO("Finished!");

    rocksdb_flushoptions_destroy(flushoptions);
    rocksdb_writeoptions_destroy(writeoptions);
    rocksdb_options_destroy(options);
    rocksdb_close(db);

    return 0;
}