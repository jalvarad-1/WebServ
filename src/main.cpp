#include "server/MultiServer.hpp"
#include "ConfigParser/Parser.hpp"
#include "server/CGI/CGI.hpp"

int main(int argc, char **argv)
{
    if (argc > 2) {
        std::cerr << "Webserv Error: Usage: " << argv[0] << " <config file>" << std::endl;
        return 1;
    }
    std::string file_name;
    if (argv[1] == NULL || *argv[1] == 0)
        file_name = "default/default.conf";
    else
        file_name = argv[1];
    std::ifstream inFileStream(file_name.c_str());
    if (!inFileStream.is_open()) {
        std::cerr << "Webserv Error: Could not open file :(" << argv[1] << std::endl;
        return 1;
    }
    std::string buffer;
    inFileStream.seekg(0, std::ios::end);
    buffer.resize(inFileStream.tellg());
    inFileStream.seekg(0, std::ios::beg);
    inFileStream.read(&buffer[0], buffer.size());
    inFileStream.close();

    try {
        simpleParser::Tokenizer tokenizer;
        std::vector<simpleParser::Token> tokens = tokenizer.parse(buffer);
        simpleParser::Parser parser;
        parser.parse(tokens);
        MultiServer multi(parser.getConfigServers());
        multi.run();
    } catch (const std::exception& e) {
        std::cerr << "Webserv Error: " << e.what() << std::endl;
        return -1;
    }
}
