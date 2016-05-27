#ifndef WOWDB_WOWOBJECT_H
#define WOWDB_WOWOBJECT_H

#include <string>
#include "leveldb/db.h"
#include "value.h"
#include "string.h"

namespace wow {
    class object : public value {
    public:
        object(leveldb::DB*, const std::string);
        value* get(const std::string) const;
        //bool has(const std::string);
        void put(const std::string, value*) const;
        void remove(const std::string) const;

        void destroy();
        void ensure();
        leveldb::DB* get_db() const;
        void duk_push(duk_context *ctx) const;
    private:
        leveldb::DB* db;
    };
}


#endif //WOWDB_WOWOBJECT_H
