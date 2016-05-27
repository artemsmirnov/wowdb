#include "gtest/gtest.h"
#include <uuid/uuid.h>
#include "store.h"

std::string test_store_execute(std::string code, std::string params) {
    uuid_t uuid;
    uuid_generate(uuid);
    char uuid_str[37];
    uuid_unparse_lower(uuid, uuid_str);

    wow::store stor("/tmp/wow_store_test_execute_" + std::string(uuid_str));

    return stor.execute(code, params);
}


TEST(Store, ReturnNumber) {
    EXPECT_EQ(
        test_store_execute(
            "(function(){return 0})",
            "{}"
        ),
        "0"
    );
}

TEST(Store, PutString) {
    EXPECT_EQ(
            test_store_execute(
                    "(function(store, root, params){\n"
                    "root.put('foo', 'bar');\n"
                    "return 'bar';\n"
                    "})",
                    "{}"
            ),
            "\"bar\""
    );
}

int main(int ac, char* av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}