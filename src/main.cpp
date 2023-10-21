#include "MultiServer.hpp"

int main()
{
    std::vector<int> ports; // Los puertos en los que quieres que tus servidores escuchen.
    ports.push_back(80);
    ports.push_back(8089);
    MultiServer multi(ports);
    multi.run();
    return 0;
}
