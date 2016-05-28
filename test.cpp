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
        "{\"result\":0}"
    );
}

TEST(Store, PutString) {
    EXPECT_EQ(
            test_store_execute(
                    "(function(store, root, params){\n"
                    "root.put('foo', 'bar');\n"
                    "return root.get('foo');\n"
                    "})",
                    "{}"
            ),
            "{\"result\":\"bar\"}"
    );
}

TEST(Store, PutNumber) {
    EXPECT_EQ(
            test_store_execute(
                    "(function(store, root, params){\n"
                            "root.put('foo', 2);\n"
                            "return root.get('foo');\n"
                            "})",
                    "{}"
            ),
            "{\"result\":2}"
    );
}

TEST(Store, RewriteValue) {
    EXPECT_EQ(
            test_store_execute(
                    "(function(store, root, params){\n"
                            "root.put('foo', 4);\n"
                            "root.put('foo', 2);\n"
                            "return root.get('foo');\n"
                            "})",
                    "{}"
            ),
            "{\"result\":2}"
    );
}

TEST(Store, GetKeys) {
    std::string keys = test_store_execute(
        "(function(store, root, params){\n"
                "root.put('foo', 2);\n"
                "root.put('bar', 4);\n"
                "root.put('baz', 5);\n"
                "return root.keys();\n"
                "})",
        "{}"
    );

    EXPECT_NE(keys.find("foo"), std::string::npos);
    EXPECT_NE(keys.find("bar"), std::string::npos);
    EXPECT_NE(keys.find("baz"), std::string::npos);
}

TEST(Store, GetUndefined) {
    EXPECT_EQ(
            test_store_execute(
                    "(function(store, root, params){\n"
                            "root.put('foo', 4);\n"
                            "root.put('foo', 2);\n"
                            "return {test: root.get('baz')};\n"
                            "})",
                    "{}"
            ),
            "{\"result\":{}}"
    );
}

TEST(Store, Remove) {
    EXPECT_EQ(
            test_store_execute(
                    "(function(store, root, params){\n"
                            "root.put('foo', 4);\n"
                            "root.remove('foo');\n"
                            "return {test: root.get('foo')};\n"
                            "})",
                    "{}"
            ),
            "{\"result\":{}}"
    );
}

TEST(Store, RemoveUndefined) {
    EXPECT_EQ(
            test_store_execute(
                    "(function(store, root, params){\n"
                            "root.put('foo', 4);\n"
                            "root.remove('bar');\n"
                            "return {test: root.get('foo')};\n"
                            "})",
                    "{}"
            ),
            "{\"result\":{\"test\":4}}"
    );
}


TEST(Store, GetKeysAfterRemove) {
    std::string keys = test_store_execute(
            "(function(store, root, params){\n"
                    "root.put('foo', 2);\n"
                    "root.put('bar', 4);\n"
                    "root.remove('foo');\n"
                    "return root.keys();\n"
                    "})",
            "{\"result\":{}}"
    );

    EXPECT_EQ(keys.find("foo"), std::string::npos);
    EXPECT_NE(keys.find("bar"), std::string::npos);
}

TEST(Store, PutObjectInObject) {
    EXPECT_EQ(
            test_store_execute(
                    "(function(store, root, params){\n"
                            "var a = store.object();\n"
                            "a.put('foo', 4);\n"
                            "root.put('a', a);;\n"
                            "return {test: root.get('a').get('foo')};\n"
                            "})",
                    "{}"
            ),
            "{\"result\":{\"test\":4}}"
    );
}

TEST(Store, ObjectHas) {
    EXPECT_EQ(
            test_store_execute(
                    "(function(store, root, params){\n"
                            "var a = store.object();\n"
                            "a.put('foo', 4);\n"
                            "root.put('a', a);;\n"
                            "return {test: root.get('a').has('foo')};\n"
                            "})",
                    "{}"
            ),
            "{\"result\":{\"test\":true}}"
    );
}

TEST(Store, ObjectHasNot) {
    EXPECT_EQ(
            test_store_execute(
                    "(function(store, root, params){\n"
                            "var a = store.object();\n"
                            "a.put('foo', 4);\n"
                            "root.put('a', a);;\n"
                            "return {test: root.get('a').has('foo1')};\n"
                            "})",
                    "{}"
            ),
            "{\"result\":{\"test\":false}}"
    );
}

TEST(Store, ArrayCreate) {
    EXPECT_EQ(
            test_store_execute(
                    "(function(store, root, params){\n"
                            "var a = store.array();\n"
                            "return {test: a.length()};\n"
                            "})",
                    "{}"
            ),
            "{\"result\":{\"test\":0}}"
    );
}

TEST(Store, ArrayInsert) {
    EXPECT_EQ(
            test_store_execute(
                    "(function(store, root, params){\n"
                            "var a = store.array();\n"
                            "a.insert(0, 10);\n"
                            "return {test: a.length(), x:a.at(0)};\n"
                            "})",
                    "{}"
            ),
            "{\"result\":{\"test\":1,\"x\":10}}"
    );
}

TEST(Store, ArrayInsertShift) {
    EXPECT_EQ(
            test_store_execute(
                    "(function(store, root, params){\n"
                            "var a = store.array();\n"
                            "a.insert(0, 10);\n"
                            "a.insert(0, 5);\n"
                            "return {test: a.length(), x:a.at(0)};\n"
                            "})",
                    "{}"
            ),
            "{\"result\":{\"test\":2,\"x\":5}}"
    );
}

TEST(Store, ArrayRemove) {
    EXPECT_EQ(
            test_store_execute(
                    "(function(store, root, params){\n"
                            "var a = store.array();\n"
                            "a.insert(0, 10);\n"
                            "a.insert(0, 5);\n"
                            "a.remove(0);\n"
                            "return {test: a.length(), x:a.at(0)};\n"
                            "})",
                    "{}"
            ),
            "{\"result\":{\"test\":1,\"x\":10}}"
    );
}

int main(int ac, char* av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}