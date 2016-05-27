#ifndef WOWDB_STORE_H
#define WOWDB_STORE_H

#include "leveldb/db.h"
#include "duktape.h"
#include "object.h"

namespace wow {
    class store {
    public:
        store(leveldb::DB*);
        std::string execute(std::string code, std::string params);
        object createObject() const;
        object root() const;
        void duk_push(duk_context *ctx) const;
    private:
        leveldb::DB* db;
    };
}


#endif //WOWDB_STORE_H
