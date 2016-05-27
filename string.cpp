#include "string.h"
#include <iostream>

namespace wow {
    string::string(leveldb::DB *_db, std::string _id) {
        id = _id;
        db = _db;
        leveldb::Status s = db->Get(leveldb::ReadOptions(), id, &val);
        assert(s.ok());
    }

    string::string(leveldb::DB *_db) {
        db = _db;
    }

    void string::destroy() {
        leveldb::Status s = db->Delete(leveldb::WriteOptions(), id);
        assert(s.ok());
        s = db->Delete(leveldb::WriteOptions(), id+"$$type");
        assert(s.ok());
    }

    void string::ensure() {
        ensure_id();

        value_type string_type = value_string;
        leveldb::Status s = db->Put(
                leveldb::WriteOptions(),
                id + "$$type",
                leveldb::Slice((const char*)&string_type, sizeof(string_type))
        );
        assert(s.ok());

        s = db->Put(
                leveldb::WriteOptions(),
                id,
                val
        );
        assert(s.ok());
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