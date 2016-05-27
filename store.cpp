#include "store.h"

namespace wow {
    const duk_function_list_entry store_methods[] = {
            //{ "object", duk_store_create_object, 0 },
            { NULL, NULL, 0 }
    };

    store::store(leveldb::DB* _db) {
        db = _db;
        object root(db, "root");
        root.ensure();
    }

    object store::root() const {
        return object(db, "root");
    }

    void store::duk_push(duk_context *ctx) const {
        // create emtpy object
        duk_push_object(ctx);

        // set db to db prop
        duk_push_pointer(ctx, (void *) db);
        duk_put_prop_string(ctx, -2, "db");

        // set methods
        duk_put_function_list(ctx, -1, store_methods);
    }
}