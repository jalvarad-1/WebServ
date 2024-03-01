
#include "Parser.hpp"

namespace simpleParser {

    Parser::Parser() {
        mTypes["location"] = Type("location", LOCATION);
        mTypes["listen"] = Type("listen", LISTEN);
        mTypes["server_names"] = Type("server_names", SERVER_NAMES);
        mTypes["error_pages"] = Type("error_pages", ERROR_PAGES);
        mTypes["root"] = Type("root", ROOT);
        mTypes["index"] = Type("index", INDEX);
        mTypes["max_body_size"] = Type("max_body_size", MAX_BODY_SIZE);
        mTypes["auto_index"] = Type("auto_index", AUTO_INDEX);
        mTypes["allowed_methods"] = Type("allowed_methods", ALLOWED_METHODS);
        mTypes["cgi_pass"] = Type("cgi_pass", CGI_PASS);
        mTypes["redirect"] = Type("redirect", REDIRECT);
    }

    void Parser::printServers() {
        for (std::vector<ServerConfig>::iterator it = _confServers.begin(); it != _confServers.end(); ++it) {
            std::cout << "--------------------------------" << std::endl;
            std::cout << "              SERVER " << std::endl;
            std::cout << "--------------------------------" << std::endl;
            it->printServerConfig();
            std::cout << "--------------------------------" << std::endl;
        }
    }

    bool Parser::expectLocationDefinition(ServerConfig & server) {
        if (mCurrentToken == mEndToken)
            return false; 
        if (mCurrentToken->mType == STRING_LITERAL && mCurrentToken->mText[0] == '/') {
            std::string key_value = mCurrentToken->mText;
            server.locations[key_value].setKeyValue(key_value);
            mCurrentToken++;
            if (expectOperator("{").mType != EMPTY) {
                std::cout << "Location block found" << std::endl;

                Type type;
                while(expectOperator("}").mType == EMPTY) {
                    type = expectType();

                    if (!expectAttributesDefinition(server , key_value, type.mType)) {
                        std::cout << "Location no configuarado correctamente" << std::endl;
                        return false;
                    }
                }
                return true;
            }

        }
        return false;
    }

    bool Parser::expectAttributesDefinition(ServerConfig & server, std::string key_value, CONF_TYPE type) {
        
        bool ret = true;
        std::string tmp;
        int error_code;
        while (expectOperator(";").mType == EMPTY ) {
            if (mCurrentToken == mEndToken)
                return false;
            switch (type) {
                case LISTEN:
                    if (key_value != "default")
                        return false;
                    ret = server.setHostAndPort( mCurrentToken->mText );
                    break;
                
                case SERVER_NAMES:
                    if (key_value != "default")
                        return false;
                    server.setServerName( mCurrentToken->mText );
                    break;

                case ERROR_PAGES:
                    if (mCurrentToken->mType != INTEGER_LITERAL)
                        return false;
                    error_code = std::atoi(mCurrentToken->mText.c_str());
                    ++mCurrentToken;
                    if (expectOperator(";").mType != EMPTY)
                        return false;
                    server.locations[key_value].setErrorPage( error_code, mCurrentToken->mText);
                    break;
                
                case ROOT:
                    ret = server.locations[key_value].setRoot( mCurrentToken->mText );
                    break;
                
                case INDEX:
                    ret = server.locations[key_value].setIndex( mCurrentToken->mText );
                    break;
                
                case MAX_BODY_SIZE:
                    if (mCurrentToken->mType != INTEGER_LITERAL)
                        return false;
                    ret = server.locations[key_value].setMaxBodySize( std::atoi(mCurrentToken->mText.c_str()) );
                    break;
                
                case AUTO_INDEX:
                    if (mCurrentToken->mText == "on")
                        server.locations[key_value].setAutoIndex( true );
                    else if (mCurrentToken->mText == "off")
                        server.locations[key_value].setAutoIndex( false );
                    else
                        return false;
                    break;
                
                case ALLOWED_METHODS:
                    ret = server.locations[key_value].setAllowedMethod( mCurrentToken->mText );
                    break;
                
                case CGI_PASS:
                    tmp = mCurrentToken->mText;
                    mCurrentToken++;
                    if (expectOperator(";").mType != EMPTY)
                        return false;
                    ret = server.locations[key_value].setCGIpass( tmp, mCurrentToken->mText );
                    break;
                case REDIRECT:
                    ret = server.locations[key_value].setRedirect( mCurrentToken->mText );
                    break;
                
                default:
                    return false;
                    break;
            }
            if (!ret)
                return false;
            mCurrentToken++;
        }
        return true;
    }

    bool Parser::expectServerAttributesDefinition( ServerConfig & server) {
        Type type = expectType();
        switch (type.mType) {
            case LOCATION:
                return expectLocationDefinition(server);
                break;
            
            case LISTEN:
            case SERVER_NAMES:
            case ERROR_PAGES:
            case ROOT:
            case INDEX:
            case MAX_BODY_SIZE:
            case AUTO_INDEX:
            case ALLOWED_METHODS:
            case CGI_PASS:
            case REDIRECT:
                return expectAttributesDefinition(server, "default", type.mType);
                break;
            
            default:
                return false;
                break;
        }
        return false;
    }

    bool Parser::expectServerDefinition() {
        if (expectIdentifier("server").mType != EMPTY) {//We have a server!!
            if (expectOperator("{").mType != EMPTY) {
                std::cout << "Server block found" << std::endl;
                ServerConfig server;
                
                while(expectOperator("}").mType == EMPTY) {

                    if (!expectServerAttributesDefinition(server)) {//We have a server attribute!!
                        std::cout << "Server no configuarado correctamente" << std::endl;// 
                        return false;
                    }
                }
                if (server.isEmptyServer())
                    return false;
                _confServers.push_back(server);
                return true;
            }
        }
        return false;
    }

    void Parser::parse(std::vector<Token> & tokens) {
        mEndToken = tokens.end();
        mCurrentToken = tokens.begin();
        
        while (mCurrentToken != mEndToken) {
            if (!expectServerDefinition()) {
                throw std::runtime_error("Unknown identifier " + mCurrentToken->mText + \
                                        " line: " + ".");
            }
        }
        if (_confServers.empty())
            throw std::runtime_error("No server definition found");
        // printServers();
    }

    Token Parser::expectOperator(const std::string & name) {
        Token emptyToken(EMPTY);
        if (mCurrentToken == mEndToken) {
            return emptyToken;
        }
        if (mCurrentToken->mType != OPERATOR) {
            return emptyToken;
        }
        if (!name.empty() && mCurrentToken->mText != name) {
            return emptyToken;
        }

        Token returnToken = *mCurrentToken;
        ++mCurrentToken;
        return returnToken;
    }

    Token Parser::expectIdentifier(const std::string & name) {
        Token emptyToken(EMPTY);
        if (mCurrentToken == mEndToken) {
            return emptyToken;
        }
        if (mCurrentToken->mType != IDENTIFIER) {
            return emptyToken;
        }
        if (!name.empty() && mCurrentToken->mText != name) {
            return emptyToken;
        }
        Token returnToken = *mCurrentToken;
        ++mCurrentToken;
        return returnToken;
    }

    Type Parser::expectType() {
        Type emptyType("", EMPTY_TYPE);
        if (mCurrentToken == mEndToken) {
            return emptyType;
        }
        if (mCurrentToken->mType != IDENTIFIER) {
            return emptyType;
        }
        std::map<std::string, Type>::iterator it = mTypes.find(mCurrentToken->mText);
        if (it == mTypes.end()) {
            return emptyType;
        }
        Type returnToken = it->second;
        ++mCurrentToken;
        return returnToken;
    }

    std::vector<ServerConfig> Parser::getConfigServers() {
        return _confServers;
    }
}
