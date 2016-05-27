#include "string.h"
#include <iostream>

namespace wow {
    string::string(leveldb::DB *_db, std::string _id) {
        id = _id;
        db = _db;
        db->Get(leveldb::ReadOptions(), id, &val);
    }

    string::string(leveldb::DB *_db) {
        db = _db;
    }

    void string::destroy() {
        db->Delete(leveldb::WriteOptions(), id);
        db->Delete(leveldb::WriteOptions(), id+"$$type");
    }

    void string::ensure() {
        ensure_id();

        value_type string_type = value_string;
        db->Put(
                leveldb::WriteOptions(),
                id + "$$type",
                leveldb::Slice((const char*)&string_type, sizeof(string_type))
        );

        db->Put(
                leveldb::WriteOptions(),
                id,
                val
        );
    }

    void string::set_value(std::string _val) {
        val = _val;
    }

    void string::duk_push(duk_context *ctx) const {
        const char* value_c_str = val.c_str();
        duk_push_string(ctx, value_c_str);
        //@TODO maybe free value_c_str
    }
}