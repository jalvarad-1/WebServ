#include "server/MultiServer.hpp"

int main()
{
    Parser parser;
    MultiServer multi(confServers);
    multi.run();
    return 0;
}