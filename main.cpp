#include <iostream>
#include "duktape.h"
#include "leveldb/db.h"
#include "object.h"

using namespace std;

leveldb::DB* db;

int main(int argc, char *argv[]) {
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testwow1", &db);
    assert(status.ok());

    //wow::store str(db);
    wow::object root(db, "root");
    root.ensure();

    duk_context *ctx = duk_create_heap_default();
    duk_eval_string(ctx, "(function(root, params){root.put('test', '123'); root.put('test', '1234'); return {root:root,test:root.get('test')}})");

    //str.duk_push(ctx);
    root.duk_push(ctx);

    duk_push_string(ctx, "{\"meaningOfLife\":42}");
    duk_json_decode(ctx, -1);

    duk_call(ctx, 2);
    printf("DUK JSON %s\n", duk_json_encode(ctx, -1));
    duk_destroy_heap(ctx);
    return 0;
}