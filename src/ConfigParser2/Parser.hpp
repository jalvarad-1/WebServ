#pragma once

#ifndef PARSER_HPP
#define PARSER_HPP

#include "Tokenizer.hpp"
#include "../ConfigParser/ServerConfig.hpp"
namespace simpleParser {

    class Parser {
    public:
        Parser();
        void parse(std::vector<Token> & tokens);
        void printServers();
    
    private:
        std::vector<ServerConfig> _confServers;

        bool expectServerDefinition();
        Type expectType();
        Token expectIdentifier(const std::string & name);

        Token expectOperator(const std::string & name);

        bool expectLocationDefinition(ServerConfig & server);
        bool expectAttributesDefinition(ServerConfig & server, std::string key_value, CONF_TYPE type);
        bool expectServerAttributesDefinition( ServerConfig & server);

        std::vector<Token>::iterator mCurrentToken;
        std::vector<Token>::iterator mEndToken;
        std::map <std::string, Type> mTypes;

    };

}

#endif

