#include <iostream>
#include "duktape.h"
#include <iostream>
#include "store.h"

using namespace std;


int main(int argc, char *argv[]) {
    wow::store str("/tmp/testwow1");

    cout << str.execute(
            "(function(store, root, params){var a=store.object();a.put('foo','bar'); a.put('a', 2); root.put('baz', a); return [root.get('baz').get('foo'), root.get('baz').get('a'), root.keys()];})",
            "{\"meaningOfLife\":42}"
    ) << endl;

    return 0;
}