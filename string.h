#ifndef WOWDB_STRING_H
#define WOWDB_STRING_H

#include <string>
#include "leveldb/db.h"
#include "value.h"

namespace wow {
    class string : public value {
    public:
        string(leveldb::DB*, std::string);
        string(leveldb::DB*);
        void set_value(std::string);

        void destroy();
        void ensure();
        void duk_push(duk_context *ctx) const;
    private:
        leveldb::DB* db;
        std::string val;
    };
}


#endif //WOWDB_STRING_H
