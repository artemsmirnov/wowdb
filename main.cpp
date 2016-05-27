#include <iostream>
#include "store.h"

using namespace std;


int main(int argc, char *argv[]) {
    wow::store stor("/tmp/test1");

    std::cout << stor.execute(
        "(function(store, root, params){var a=store.object();a.put('foo','bar'); a.put('a', 2); root.put('baz', a); return [root.get('baz').get('foo'), root.get('baz').get('a'), root.keys()];})",
        "{\"meaningOfLife\":42}"
    ) << std::endl;

    return 0;
}