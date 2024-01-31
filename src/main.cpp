#include "server/MultiServer.hpp"

#include "ConfigParser/Parser.hpp"
int main(int argc, char **argv)
{
    std::cout << "simpleparser 0.1\n" << std::endl;
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

    simpleParser::Tokenizer tokenizer;
    std::vector<simpleParser::Token> tokens = tokenizer.parse(buffer);
    for (size_t i = 0; i < tokens.size(); i++) {
        tokens[i].debugPrint();
    }
    simpleParser::Parser parser;
    parser.parse(tokens);



    //std::string buffer;
    //exit(1);
    //try {
    //    ConfigParser serverConfig(argv[1]);
    //    MultiServer multi(serverConfig.getConfigServers());
    //    multi.run();
    //    (void)argc;
    //    (void)argv;
    //    return 0;
    //} catch (const std::exception& e) {
    //    std::cerr << "Error: " << e.what() << std::endl;
    //    return -1;
    //}
}

    //   std::ifstream inFileStream(resource_path.c_str());
    //
    //if (inFileStream.is_open())
    //{
    //    std::string		buffer;
	//	inFileStream.seekg(0, std::ios::end);
	//	buffer.resize(inFileStream.tellg());
	//	inFileStream.seekg(0, std::ios::beg);
	//	inFileStream.read(&buffer[0], buffer.size());
    //    inFileStream.close();
    //    response.response_code = 200;
    //    response.string_body = buffer;
    //    return response;
    //}
