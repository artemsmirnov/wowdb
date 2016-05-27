#ifndef WOWDB_STORE_H
#define WOWDB_STORE_H

#include "leveldb/db.h"
#include "duktape.h"
#include "object.h"

namespace wow {
    class store {
    public:
        store(leveldb::DB*);
        object root() const;
        void duk_push(duk_context *ctx) const;
    private:
        leveldb::DB* db;
    };
}


#endif //WOWDB_STORE_H
