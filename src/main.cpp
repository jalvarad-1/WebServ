#include "server/MultiServer.hpp"

#include "ConfigParser/Parser.hpp"
int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config file>" << std::endl;
        return 1;
    }
    std::ifstream inFileStream(argv[1]);

    if (!inFileStream.is_open()) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    std::string buffer;
    inFileStream.seekg(0, std::ios::end);
    buffer.resize(inFileStream.tellg());
    inFileStream.seekg(0, std::ios::beg);
    inFileStream.read(&buffer[0], buffer.size());
    inFileStream.close();
    std::cout << "File content:\n" << buffer << std::endl;

    try {
        simpleParser::Tokenizer tokenizer;
        std::vector<simpleParser::Token> tokens = tokenizer.parse(buffer);
        for (size_t i = 0; i < tokens.size(); i++) {
            tokens[i].debugPrint();
        }
        simpleParser::Parser parser;
        parser.parse(tokens);
        MultiServer multi(parser.getConfigServers());
        multi.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}
