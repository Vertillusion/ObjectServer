#include <drogon/HttpAppFramework.h>
int main(int argc, const char** argv) {
    std::string ip;
    int port;
    if (argc >= 2) ip = argv[1]; else ip = "127.0.0.1";
    if (argc >= 3) port = std::stoi(argv[2]); else port = 80;

    drogon::app().addListener("127.0.0.1", 8080);

    drogon::app()
        .run();
    return 0;
}