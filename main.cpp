#include <iostream>
#include "leveldb/db.h"
#include "store.h"

using namespace std;

leveldb::DB* db;

int main(int argc, char *argv[]) {
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testwow1", &db);
    assert(status.ok());

    wow::store stor(db);

    std::cout << stor.execute(
        "(function(store, root, params){var a=store.object();a.put('foo','bar'); a.put('a', 2); root.put('baz', a); return [root.get('baz').get('foo'), root.get('baz').get('a'), root.keys()];})",
        "{\"meaningOfLife\":42}"
    ) << std::endl;

    return 0;
}