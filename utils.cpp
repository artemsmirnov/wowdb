#include "utils.h"
#include "string.h"
#include "object.h"
#include "number.h"
#include "undefined.h"

namespace wow {
    value* duk_get_value(leveldb::DB* db, duk_context *ctx, int idx) {
        if (duk_check_type(ctx, idx, DUK_TYPE_STRING)) {
            const char *raw_str = duk_get_string(ctx, idx);
            string* val = new string(db);
            val->set_value(raw_str);
            return val;
        }

        if (duk_check_type(ctx, idx, DUK_TYPE_NUMBER)) {
            double num = duk_get_number(ctx, idx);
            number* val = new number(db);
            val->set_value(num);
            return val;
        }

        if (duk_check_type(ctx, idx, DUK_TYPE_OBJECT)) {
            // @TODO check .type property

            duk_get_prop_string(ctx, idx, "id");
            const char *id = duk_get_string(ctx, idx);

            duk_get_prop_string(ctx, idx-1, "db");
            leveldb::DB* db = (leveldb::DB*) duk_get_pointer(ctx, -1);

            duk_pop_2(ctx); // [key]

            return new object(db, id);
        }

        return new undefined();
    }

    value* get_value_by_id(leveldb::DB* db, std::string value_id) {
        std::string type_value;
        leveldb::Status s = db->Get(leveldb::ReadOptions(), value_id + "$$type", &type_value);
        assert(s.ok());

        value_type *type = (wow::value_type*)type_value.c_str();  // @TODO maybe we need free there
        switch(*type) {
            case value_string:
                return new string(db, value_id);
            case value_number:
                return new number(db, value_id);
            case value_object:
                return new object(db, value_id);
            default:
                return new undefined();
        }
    }
}