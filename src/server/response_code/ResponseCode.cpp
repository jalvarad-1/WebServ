#include "ResponseCode.hpp"

ResponseCode::ResponseCode() {
    this->_codes[200] = "Ok";
    this->_codes[302] = "Found";
    this->_codes[400] = "Bad Request";
    this->_codes[404] = "Not Found";
    this->_codes[405] = "Method Not Allowed";
    this->_codes[501] = "Not implemented";
    this->_codes[500] = "Internal Server Error";
}

std::string ResponseCode::getCodeString(int code) {
    return (this->_codes[code]);
}

ResponseCode::~ResponseCode() {}
