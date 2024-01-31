#pragma once
#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP
#include <vector>
#include <map>
#include <iostream>

namespace simpleParser {
    enum TokenType {
        WHITESPACE, // No token ever has this type
        IDENTIFIER,
        INTEGER_LITERAL,
        STRING_LITERAL,
        OPERATOR,
        COMMENT,
        EMPTY, // No token ever has this type
    };

    static const char* sTokenTypeStrings[] = {
        "WHITESPACE",
        "IDENTIFIER",
        "INTEGER_LITERAL",
        "STRING_LITERAL",
        "OPERATOR",
        "COMMENT",
        "EMPTY"
    };

    class Token {
    public:
        TokenType mType;
        std::string mText;
        size_t mLineNumber;

        Token();
        Token(TokenType type);
        void debugPrint() const;
    };

    class Tokenizer
    {
        private:
            void endToken(Token & token, std::vector<Token> & tokens);
        
        public:
            std::vector<Token> parse(const std::string & inProgram);
    };

    enum CONF_TYPE{
        LOCATION,
        LISTEN,
        SERVER_NAMES,
        ERROR_PAGES,
        ROOT,
        INDEX,
        MAX_BODY_SIZE,
        AUTO_INDEX,
        ALLOWED_METHODS,
        CGI_PASS,
        EMPTY_TYPE
    };

    class Type {
    public:
        Type(){};
        Type(const std::string & name, CONF_TYPE type) : mName(name), mType(type) {}
        std::string mName;
        CONF_TYPE mType;

    };
}

#endif