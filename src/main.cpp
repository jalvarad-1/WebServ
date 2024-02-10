#include "server/MultiServer.hpp"
#include "ConfigParser/Parser.hpp"
#include "server/CGI/CGI.hpp"

int main(int argc, char **argv)
{
	ñapaCounter = 0;
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

// int main() {
//     std::string cgi_path = "cgi_bin/cgi_python";
//     std::string file_path = "cgi_bin/script.py";
//     CGI_Return ret;

//     // Este va a ser el mapa con el que le vamos a pasar todas las variables de entorno.
//     std::map<std::string, std::string> env;
//     env["REQUEST_METHOD"] = "GET";
//     env["SERVER_PROTOCOL"] = "HTTP/1.1";
//     env["PATH_INFO"] = " ";

//     // Este va a ser el vector con el que le vamos a pasar los argumentos.
//     std::vector<std::string> args;
//     args.push_back("/home/pavon/WebServ/cgi_bin/cgi_python");

//     CGI cgi(cgi_path, file_path);
//     cgi.set_env(env);
//     cgi.set_args(args);
//     ret = cgi.run_CGI();

//     std::cout << "Header Status: " << ret.headers["Status"] << std::endl;

//     std::cout << "Body: " << ret.body << std::endl;
//     //Guardamos en ret los valores que obtenemos del CGI.

//     return 0;
// }
