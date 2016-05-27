#ifndef WOWDB_NUMBER_H
#define WOWDB_NUMBER_H

#include "leveldb/db.h"
#include "value.h"

namespace wow {
    class number : public value {
    public:
        number(leveldb::DB*, std::string);
        number(leveldb::DB*);
        void set_value(double);

        void destroy();
        void ensure();
        void duk_push(duk_context *ctx) const;
    private:
        leveldb::DB* db;
        double val;
    };
}



#endif //WOWDB_NUMBER_H
