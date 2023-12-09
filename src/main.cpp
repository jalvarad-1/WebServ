#include "server/MultiServer.hpp"


int main(int argc, char **argv)
{
    try {
        ConfigParser serverConfig(argv[1]);
        MultiServer multi(serverConfig.getConfigServers());
        multi.run();
        (void)argc;
        (void)argv;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}
