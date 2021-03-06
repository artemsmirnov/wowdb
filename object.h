#ifndef WOWDB_OBJECT_H
#define WOWDB_OBJECT_H

#include <string>
#include <vector>
#include "leveldb/db.h"
#include "value.h"
#include "string.h"

namespace wow {
    class object : public value {
    public:
        object(leveldb::DB*);
        object(leveldb::DB*, const std::string);
        value* get(const std::string) const;
        std::vector<std::string> keys() const;
        bool has(const std::string);
        void put(const std::string, value*);
        void remove(const std::string) const;

        void destroy();
        void ensure();
        leveldb::DB* get_db() const;
        void duk_push(duk_context *ctx) const;
    private:
        leveldb::DB* db;
    };
}


#endif //WOWDB_OBJECT_H
