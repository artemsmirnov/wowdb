#include "number.h"
#include <string>

namespace wow {
    number::number(leveldb::DB *_db, std::string _id) {
        id = _id;
        db = _db;
        std::string raw_val;
        db->Get(leveldb::ReadOptions(), id, &raw_val);
        val = *(double*)raw_val.c_str(); // @TODO maybe we need free there
    }

    number::number(leveldb::DB *_db) {
        db = _db;
    }

    void number::destroy() {
        db->Delete(leveldb::WriteOptions(), id);
        db->Delete(leveldb::WriteOptions(), id+"$$type");
    }

    void number::ensure() {
        ensure_id();

        value_type number_type = value_number;
        db->Put(
                leveldb::WriteOptions(),
                id + "$$type",
                leveldb::Slice((const char*)&number_type, sizeof(number_type))
        );

        db->Put(
                leveldb::WriteOptions(),
                id,
                leveldb::Slice((const char*)&val, sizeof(val))
        );
    }

    void number::set_value(double _val) {
        val = _val;
    }

    void number::duk_push(duk_context *ctx) const {
        duk_push_number(ctx, val);
    }
}