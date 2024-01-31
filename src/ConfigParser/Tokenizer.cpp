
#include "Tokenizer.hpp"

namespace simpleParser{
    Token::Token() : mType(WHITESPACE), mText(""), mLineNumber(0) {}
    Token::Token(TokenType type) : mType(type), mText(""), mLineNumber(0){}

    std::vector<Token> Tokenizer::parse(const std::string & inProgram) {
        std::vector<Token> tokens;
        Token currentToken;
        bool hasIdentifier = false;

        for (size_t i = 0; i < inProgram.size(); i++) {
            if (currentToken.mType == COMMENT) {
                if (inProgram[i] == '\n') {
                    endToken(currentToken, tokens);
                    ++currentToken.mLineNumber;
                } else {
                    currentToken.mText.append(1, inProgram[i]);
                }
                continue;
            }
            switch (inProgram[i]) {
                case '0' ... '9':
                    if (currentToken.mType == WHITESPACE) {
                        currentToken.mType = INTEGER_LITERAL;
                    }
                    currentToken.mText.append(1, inProgram[i]); 
                    break;
                case '{':
                case '}':
                case ';':
                    endToken(currentToken, tokens);
                    currentToken.mType = OPERATOR;
                    currentToken.mText.append(1, inProgram[i]);
                    endToken(currentToken, tokens);
                    if (inProgram[i] == ';' || inProgram[i] == '{')
                        hasIdentifier = false;
                    break;
                
                case ' ':
                case '\t':
                    if (currentToken.mType == COMMENT) {
                        currentToken.mText.append(1, inProgram[i]);
                    } else {
                        endToken(currentToken, tokens);
                    }
                    break;
                
                case '\n':
                case '\r':
                    endToken(currentToken, tokens);
                    ++currentToken.mLineNumber;
                    break;
                
                case '#':
                    endToken(currentToken, tokens);
                    currentToken.mType = COMMENT;
                    currentToken.mText.append(1, inProgram[i]);
                    break;
                
                default:
                    if (currentToken.mType == WHITESPACE) {
                        endToken(currentToken, tokens);
                        currentToken.mType = hasIdentifier ? STRING_LITERAL : IDENTIFIER;
                        hasIdentifier = currentToken.mType == IDENTIFIER;
                    } else if (currentToken.mType == INTEGER_LITERAL && !isdigit(inProgram[i])) {
                        currentToken.mType = hasIdentifier ? STRING_LITERAL : IDENTIFIER;
                        hasIdentifier = currentToken.mType == IDENTIFIER;
                    }
                    currentToken.mText.append(1, inProgram[i]);
                    break;
            }
        }
        endToken(currentToken, tokens);
        return tokens;
    }

    void Tokenizer::endToken(Token & token, std::vector<Token> & tokens) {
        if (token.mType == COMMENT) {
            std::cout << "Ignoring comment" << token.mText << std::endl;
        }
        else if (token.mType != WHITESPACE) {
            tokens.push_back(token);
        }
        token.mType = WHITESPACE;
        token.mText.erase();
    }

    void Token::debugPrint() const {
        std::cout << "Token(" << sTokenTypeStrings[mType] << ", \"" << mText << "\", " << mLineNumber << ")" << std::endl;
    }
}
    