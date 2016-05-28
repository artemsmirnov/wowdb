#include "array.h"
#include "undefined.h"
#include "utils.h"
#include <boost/algorithm/string.hpp>


namespace wow {
    // duktape adapter
    array duk_get_this_array(duk_context *ctx) {
        duk_push_this(ctx); // [this]

        duk_get_prop_string(ctx, -1, "id"); // [this this.id]
        const char *id = duk_get_string(ctx, -1);

        duk_get_prop_string(ctx, -2, "db"); // [this this.id this.db]
        leveldb::DB* db = (leveldb::DB*) duk_get_pointer(ctx, -1);

        duk_pop_3(ctx); // []

        return array(db, id);
    }

    duk_ret_t duk_array_length(duk_context *ctx) {
        array arr = duk_get_this_array(ctx);
        duk_push_int(ctx, arr.length());

        return 1;
    }

    duk_ret_t duk_array_at(duk_context *ctx) {
        array arr = duk_get_this_array(ctx);
        int idx = duk_get_int(ctx, -1);
        value *val = arr.at(idx);

        val->duk_push(ctx);

        delete val;

        return 1;
    }

    duk_ret_t duk_array_insert(duk_context *ctx) {
        array arr = duk_get_this_array(ctx);

        int idx = duk_get_int(ctx, -2);
        value *val = duk_get_value(arr.get_db(), ctx, -1);
        arr.insert(idx, val);

        return 0;
    }

    duk_ret_t duk_array_remove(duk_context *ctx) {
        array arr = duk_get_this_array(ctx);
        int idx = duk_get_int(ctx, -1);
        arr.remove(idx);

        return 0;
    }


    const duk_function_list_entry wow_array_methods[] = {
            { "length", duk_array_length, 0 },
            { "at", duk_array_at, 1 },
            { "insert", duk_array_insert, 2 },
            { "remove", duk_array_remove, 1 },
            { NULL, NULL, 0 }
    };

    array::array(leveldb::DB* _db, std::string _id) {
        db = _db;
        id = _id;
        std::string raw_vals;

        leveldb::Status s = db->Get(leveldb::ReadOptions(), id, &raw_vals);
        assert(s.ok());

        boost::split(vals, raw_vals, boost::is_any_of(","));
        if (vals[0] == "") {
            vals.erase(vals.begin());
        }
    }

    array::array(leveldb::DB* _db) {
        db = _db;
        vals = std::vector<std::string>();
    }

    unsigned long array::length() const {
        return vals.size();
    }

    value* array::at(int idx) const {
        if (vals.size() <= idx || idx < 0) {
            return new undefined();
        }

        return get_value_by_id(db, vals[idx]);
    }

    void array::insert(int idx, value *val) {
        val->ensure();
        vals.insert(vals.begin()+idx, val->get_id());
        ensure();
    }

    void array::remove(int idx) {
        vals.erase(vals.begin()+idx);
        ensure();
    }

    void array::destroy() {
        //@TODO
    }

    void array::ensure() {
        ensure_id();
        value_type array_type = value_array;

        leveldb::Status s = db->Put(
                leveldb::WriteOptions(),
                id + "$$type",
                leveldb::Slice((const char*)&array_type, sizeof(array_type))
        );
        assert(s.ok());

        s = db->Put(
                leveldb::WriteOptions(),
                id,
                boost::algorithm::join(vals, ",")
        );
        assert(s.ok());
    }

    leveldb::DB* array::get_db() const {
        return db;
    }


    void array::duk_push(duk_context *ctx) const {
        const char* _id = id.c_str();

        // create emtpy object
        duk_push_object(ctx);

        // set id to id prop
        duk_push_string(ctx, _id);
        duk_put_prop_string(ctx, -2, "id");

        // set db to db prop
        duk_push_pointer(ctx, (void *) db);
        duk_put_prop_string(ctx, -2, "db");

        // set 'object' to type prop
        duk_push_string(ctx, "array");
        duk_put_prop_string(ctx, -2, "type");

        // set methods
        duk_put_function_list(ctx, -1, wow_array_methods);
    }
}