#ifndef WOWDB_UNDEFINED_H
#define WOWDB_UNDEFINED_H

#include "value.h"

namespace wow {
    class undefined : public value {
        void destroy();
        void ensure();
        void duk_push(duk_context *ctx) const;
    };
}

#endif //WOWDB_UNDEFINED_H
