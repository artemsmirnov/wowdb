#include <string>
#include "value.h"
#include <uuid/uuid.h>

namespace wow {
    std::string generate_uuid() {
        uuid_t uuid;
        uuid_generate(uuid);
        char uuid_str[37];
        uuid_unparse_lower(uuid, uuid_str);
        return std::string(uuid_str);
    }

    void value::ensure_id() {
        if (id.empty()) {
            id = generate_uuid();
        }
    }

    std::string value::get_id() const {
        return id;
    }
}