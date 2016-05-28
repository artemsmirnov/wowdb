#ifndef WOWDB_ARRAY_H
#define WOWDB_ARRAY_H

#include <vector>
#include "leveldb/db.h"
#include "value.h"

namespace wow {
    class array : public value {
    public:
        array(leveldb::DB*);
        array(leveldb::DB*, const std::string);
        unsigned long length() const;
        value* at(int) const;
        void insert(int, value*);
        void remove(int);

        void destroy();
        void ensure();
        leveldb::DB* get_db() const;
        void duk_push(duk_context *ctx) const;
    private:
        leveldb::DB* db;
        std::vector<std::string> vals;
    };
}

#endif //WOWDB_ARRAY_H
