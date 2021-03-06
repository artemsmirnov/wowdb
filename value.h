#ifndef WOWDB_VALUE_H
#define WOWDB_VALUE_H

#include <string>
#include "duktape.h"

namespace wow {
    enum value_type {
        value_object = 0,
        value_array = 1,
        value_number = 2,
        value_string = 4
    };


    class value {
    public:
        virtual void destroy() = 0;
        virtual void ensure() = 0;
        std::string get_id() const;
        virtual void duk_push(duk_context *ctx) const = 0;
    protected:
        std::string id;
        void ensure_id();
    };
}


#endif //WOWDB_VALUE_H
