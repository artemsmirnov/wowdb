#include <iostream>
#include "duktape.h"
#include "leveldb/db.h"
#include <uuid/uuid.h>

using namespace std;

enum wow_type {
    wow_object = 0,
    wow_array = 1,
    wow_int = 2,
    wow_double = 3,
    wow_string = 4
};

leveldb::DB* db;

void wow_push_value(duk_context *ctx, const char *_id);

duk_ret_t wow_object_get(duk_context *ctx) {
    // [key]

    duk_push_this(ctx); // [key this]
    duk_get_prop_string(ctx, -1, "_id"); // [key this this._id]
    const char *_id = duk_get_string(ctx, -1);
    duk_pop_2(ctx); // [key]
    const char *key = duk_get_string(ctx, -1);
    char absolute_key[256];
    strcpy(absolute_key, _id);
    strcat(absolute_key, ".");
    strcat(absolute_key, key);

    std::string value_key;
    leveldb::Status s = db->Get(leveldb::ReadOptions(), absolute_key, &value_key);

    wow_push_value(ctx, value_key.c_str());

    return 1;
}

duk_ret_t wow_object_set(duk_context *ctx) {
    // @TODO handle value change
    // [key value]

    duk_push_this(ctx); // [key value this]
    duk_get_prop_string(ctx, -1, "_id"); // [key value this this._id]
    const char *_id = duk_get_string(ctx, -1);
    duk_pop_2(ctx); // [key value]
    const char *key = duk_get_string(ctx, -2);
    char absolute_key[256];
    strcpy(absolute_key, _id);
    strcat(absolute_key, ".");
    strcat(absolute_key, key);

    // @TODO check if uuid is unique
    uuid_t uuid;
    uuid_generate(uuid);
    char uuid_str[37];
    uuid_unparse_lower(uuid, uuid_str);

    // @TODO store not only strings
    const char *value = duk_get_string(ctx, -1);


    wow_type val_type = wow_string;

    char type_key[256];
    strcpy(type_key, uuid_str);
    strcat(type_key, "$$type");

    db->Put(leveldb::WriteOptions(), absolute_key, uuid_str);
    db->Put(leveldb::WriteOptions(), type_key, leveldb::Slice((const char*)&val_type, sizeof(val_type)));
    db->Put(leveldb::WriteOptions(), uuid_str, value);

    return 0;
}

duk_ret_t wow_object_has(duk_context *ctx) {
    duk_push_number(ctx, 10);
    return 1;
}

duk_ret_t wow_object_remove(duk_context *ctx) {
    duk_push_number(ctx, 10);
    return 1;
}

const duk_function_list_entry wow_object_methods[] = {
        { "get", wow_object_get, 1 },
        { "set", wow_object_set, 2 },
        { "has", wow_object_has, 1 },
        { "remove", wow_object_remove, 1 },
        { NULL, NULL, 0 }
};

void wow_push_object(duk_context *ctx, const char *key) {
    duk_push_object(ctx);
    duk_push_string(ctx, key);
    duk_put_prop_string(ctx, -2, "_id");

    duk_put_function_list(ctx, -1, wow_object_methods);
}

void wow_push_string(duk_context *ctx, const char *key) {
    string value;
    db->Get(leveldb::ReadOptions(), key, &value);
    duk_push_string(ctx, value.c_str());
}

void wow_push_value(duk_context *ctx, const char *_id) {
    char type_key[256];
    strcpy(type_key, _id);
    strcat(type_key, "$$type");
    string type_value;
    db->Get(leveldb::ReadOptions(), type_key, &type_value);

    wow_type *type = (wow_type*)type_value.c_str();

    switch(*type) {
        case wow_object:
            wow_push_object(ctx, _id);
            return;
        case wow_string:
            wow_push_string(ctx, _id);
            return;
        default:
            duk_push_undefined(ctx);
    }
}

int main(int argc, char *argv[]) {
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb1", &db);
    assert(status.ok());

    wow_type root_type = wow_object;
    db->Put(leveldb::WriteOptions(), "root$$type", leveldb::Slice((const char*)&root_type, sizeof(root_type)));


    duk_context *ctx = duk_create_heap_default();
    duk_eval_string(ctx, "(function(root, params){root.set('test', '123'); root.set('test', '1234'); return {test:root.get('test')}})");

    wow_push_value(ctx, "root");

    duk_push_string(ctx, "{\"meaningOfLife\":42}");
    duk_json_decode(ctx, -1);

    duk_call(ctx, 2);
    printf("DUK JSON %s\n", duk_json_encode(ctx, -1));
    duk_destroy_heap(ctx);
    return 0;
}