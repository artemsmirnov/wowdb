#include "store.h"

namespace wow {
    // duktape adapter
    duk_ret_t duk_store_create_object(duk_context *ctx) {
        // [key]
        duk_push_this(ctx); // [key this]

        duk_get_prop_string(ctx, -1, "store"); // [key this this.id this.db]
        store* str = (store*) duk_get_pointer(ctx, -1);
        duk_pop_2(ctx); // [key]

        str->createObject().duk_push(ctx);

        return 1;
    }

    const duk_function_list_entry store_methods[] = {
            { "object", duk_store_create_object, 0 },
            { NULL, NULL, 0 }
    };

    store::store(leveldb::DB* _db) {
        db = _db;

        object root(db, "root");
        root.ensure();
    }

    store::store(std::string path) {
        leveldb::Options options;
        options.create_if_missing = true;
        leveldb::Status status = leveldb::DB::Open(options, path, &db);
        assert(status.ok());

        object root(db, "root");
        root.ensure();
    }

    store::~store() {
        delete db;
    }

    std::string store::execute(std::string code, std::string params) {
        duk_context *ctx = duk_create_heap_default();
        duk_eval_string(ctx, code.c_str());

        duk_push(ctx);
        root().duk_push(ctx);

        duk_push_string(ctx, params.c_str());
        duk_json_decode(ctx, -1);

        duk_call(ctx, 3);
        std::string res = duk_json_encode(ctx, -1);
        duk_destroy_heap(ctx);

        return res;
    }

    object store::createObject() const {
        object obj(db);
        obj.ensure();
        return obj;
    }

    object store::root() const {
        return object(db, "root");
    }

    void store::duk_push(duk_context *ctx) const {
        // create emtpy object
        duk_push_object(ctx);

        // set db to db prop
        duk_push_pointer(ctx, (void *) this);
        duk_put_prop_string(ctx, -2, "store");

        // set methods
        duk_put_function_list(ctx, -1, store_methods);
    }
}