#include "undefined.h"

namespace wow {
    void undefined::destroy() {

    }

    void undefined::ensure() {

    }

    void undefined::duk_push(duk_context *ctx) const {
        duk_push_undefined(ctx);
    }
}