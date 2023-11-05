#include "MultiServer.hpp"
int main(int argc, char **argv)
{
    try {
        ConfigParser serverConfig(argv[1]);
        std::vector<int> ports; // Los puertos en los que quieres que tus servidores escuchen.
        ports.push_back(80);
        ports.push_back(8089);
        MultiServer multi(ports);
        // multi.run();
        (void)argc;
        (void)argv;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}
