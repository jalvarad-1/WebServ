# WEBSERV

#### Create an http server from scratch in c++. We learn in depth the concepts of websockets.

#### The group is made up of [jalvarad-1](https://github.com/jalvarad-1), [cristiana-marcu](https://github.com/cristiana-marcu) and [dpavon-g](https://github.com/dpavon-g).

---

## Project conditions

> Full details are included in the `WebServPDF.pdf` file.

## HTTPRequest Class
The HTTPRequest class is a representation of an [HTTP request](https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages#http_requests), providing a structured way to access the different components of the request, such as the method, URI, HTTP version, headers and body. This class only checks basic formatting and does not check if there is a virtual server registered with that uri and that method is accepted.

Main features:

An HTTPRequest object can be constructed from an HTTP request in raw (string) format (raw_request).

Access to Request Components: The class provides methods to easily access the method, URI, HTTP version, headers and body of the request.

Verification of Methods with Body: You can verify whether a particular method accepts a body or not using the methodAcceptsBody method.

Automatic Parsing: During object construction, the class automatically parses the raw request, extracting and storing the components in their respective member variables.

Error Handling: If an error occurs during parsing, an error message is stored and can be retrieved using the getErrorMessage method.

Basic use:
```c++
#include "HTTPRequest.hpp"
int main()
{
    std::string raw_request = "GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n";
    HTTPRequest request(raw_request);

    std::cout << "Method: " << request.getMethod() << std::endl;
    std::cout << "URI: " << request.getURI() << std::endl;
    ...
}
```

## Documentation

See the complete documentation of the project in the wiki of this repository or by clicking [here]().

