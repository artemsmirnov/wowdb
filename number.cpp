#include "number.h"
#include <string>

namespace wow {
    number::number(leveldb::DB *_db, std::string _id) {
        id = _id;
        db = _db;
        std::string raw_val;
        leveldb::Status s = db->Get(leveldb::ReadOptions(), id, &raw_val);
        assert(s.ok());
        val = *(double*)raw_val.c_str();
    }

    number::number(leveldb::DB *_db) {
        db = _db;
    }

    void number::destroy() {
        leveldb::Status s = db->Delete(leveldb::WriteOptions(), id);
        assert(s.ok());
        s = db->Delete(leveldb::WriteOptions(), id+"$$type");
        assert(s.ok());
    }

    void number::ensure() {
        ensure_id();

        value_type number_type = value_number;
        leveldb::Status s = db->Put(
                leveldb::WriteOptions(),
                id + "$$type",
                leveldb::Slice((const char*)&number_type, sizeof(number_type))
        );
        assert(s.ok());

        s = db->Put(
                leveldb::WriteOptions(),
                id,
                leveldb::Slice((const char*)&val, sizeof(val))
        );
        assert(s.ok());
    }

    void number::set_value(double _val) {
        val = _val;
    }

    void number::duk_push(duk_context *ctx) const {
        duk_push_number(ctx, val);
    }
}