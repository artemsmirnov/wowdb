#ifndef WOWDB_UTILS_H
#define WOWDB_UTILS_H

#include "leveldb/db.h"
#include <duktape.h>
#include "value.h"

namespace wow {
    value* duk_get_value(leveldb::DB* db, duk_context *ctx, int idx);
    value* get_value_by_id(leveldb::DB* db, std::string value_id); // maybe move this into store
}

#endif //WOWDB_UTILS_H
