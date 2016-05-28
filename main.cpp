#include <iostream>
#include "duktape.h"
#include <iostream>
#include "store.h"
#include "lib/crow.h"
#include <sstream>
#include <string>

using namespace std;

/*
int main(int argc, char *argv[]) {
    wow::store str("/tmp/testwow1");

    cout << str.execute(
            "(function(store, root, params){var a=store.object();a.put('foo','bar'); a.put('a', 2); root.put('baz', a); return [root.get('baz').get('foo'), root.get('baz').get('a'), root.keys()];})",
            "{\"meaningOfLife\":42}"
    ) << endl;

    return 0;
}
 */

wow::store *stor;

int main(int argc, char *argv[]) {

    istringstream port_stream(argv[1]);
    int port;
    if (!(port_stream >> port)) {
        cout << "wowdb [port] [path]\n";
        return 1;
    }

    istringstream path_stream(argv[2]);
    string path;
    if (!(path_stream >> path)) {
        cout << "wowdb [port] [path]\n";
        return 1;
    }


    crow::SimpleApp app;
    stor = new wow::store(path);

    CROW_ROUTE(app, "/").methods("POST"_method)
    ([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400);
        }

        cout << body["code"].s() << endl;

        return crow::response(
            stor->execute(
                body["code"].s(),
                body["params"].s()
            )
        );
    });

    app.port(port).run();
}