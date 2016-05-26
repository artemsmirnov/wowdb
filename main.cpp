#include <iostream>
#include <duktape.h>

using namespace std;


int main(int argc, char *argv[]) {
    duk_context *ctx = duk_create_heap_default();
    duk_eval_string(ctx, "print('Hello world!');");
    duk_destroy_heap(ctx);
    return 0;
}