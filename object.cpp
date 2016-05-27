#include "object.h"

namespace wow {
    // duktape adapter
    object duk_get_this_object(duk_context *ctx) {
        // [key]
        duk_push_this(ctx); // [key this]

        duk_get_prop_string(ctx, -1, "id"); // [key this this.id]
        const char *id = duk_get_string(ctx, -1);

        duk_get_prop_string(ctx, -2, "db"); // [key this this.id this.db]
        leveldb::DB* db = (leveldb::DB*) duk_get_pointer(ctx, -1);

        duk_pop_3(ctx); // [key]

        return object(db, id);
    }

    value* duk_get_value(leveldb::DB* db, duk_context *ctx, int idx) {
        if (duk_check_type(ctx, idx, DUK_TYPE_STRING)) {
            const char *raw_str = duk_get_string(ctx, -1);
            string* val = new string(db);
            val->set_value(raw_str);
            return val;
        }

        if (duk_check_type(ctx, idx, DUK_TYPE_OBJECT)) {
            // @TODO check .type property

            duk_get_prop_string(ctx, idx, "id");
            const char *id = duk_get_string(ctx, -1);

            duk_get_prop_string(ctx, idx-1, "db");
            leveldb::DB* db = (leveldb::DB*) duk_get_pointer(ctx, -1);

            duk_pop_2(ctx); // [key]

            return new object(db, id);
        }
    }

    duk_ret_t duk_object_get(duk_context *ctx) {
        object obj = duk_get_this_object(ctx);
        const char *key = duk_get_string(ctx, -1);
        value *val = obj.get(key);

        val->duk_push(ctx);

        delete val;

        return 1;
    }

    duk_ret_t wow_object_put(duk_context *ctx) {
        object obj = duk_get_this_object(ctx);

        const char *key = duk_get_string(ctx, -2);
        value *val = duk_get_value(obj.get_db(), ctx, -1);
        obj.put(key, val);

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
            { "get", duk_object_get, 1 },
            { "put", wow_object_put, 2 },
            { "has", wow_object_has, 1 },
            { "remove", wow_object_remove, 1 },
            { NULL, NULL, 0 }
    };

    // class methods

    object::object(leveldb::DB* _db, std::string _id) {
        db = _db;
        id = _id;
    }

    value* object::get(std::string key) const {
        std::string value_id;
        leveldb::Status s = db->Get(leveldb::ReadOptions(), id + "." + key, &value_id);

        // determine by type value concrete class

        std::string type_value;
        db->Get(leveldb::ReadOptions(), value_id + "$$type", &type_value);

        value_type *type = (wow::value_type*)type_value.c_str();
        switch(*type) {
            case value_string:
                return new string(db, value_id);
            case value_object:
                return new object(db, value_id);
        }
    }

    void object::put(const std::string key, value* val) const {
        get(key)->destroy();
        val->ensure();
        db->Put(leveldb::WriteOptions(), id+"."+key, val->get_id());
    }

    void object::remove(const std::string key) const {
        get(key)->destroy();
        db->Delete(leveldb::WriteOptions(), id+"."+key);
    }

    void object::destroy() {
        //@TODO
        // get all keys
        // remove all keys
        // remove type key
    }

    void object::ensure() {
        ensure_id();

        value_type object_type = value_object;
        db->Put(
                leveldb::WriteOptions(),
                id + "$$type",
                leveldb::Slice((const char*)&object_type, sizeof(object_type))
        );
    }

    leveldb::DB* object::get_db() const {
        return db;
    }

    void object::duk_push(duk_context *ctx) const {
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
        duk_push_string(ctx, "object");
        duk_put_prop_string(ctx, -2, "type");

        // set methods
        duk_put_function_list(ctx, -1, wow_object_methods);

        // @TODO maybe we need to free(_id) here
    }
}