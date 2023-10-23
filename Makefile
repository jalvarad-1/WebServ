CXX := g++
NAME := webserv
CXXFLAGS := -Wall -Werror -Wextra -std=c++98
SRCS =	src/main.cpp\
		src/BindingSocket.cpp\
		src/HTTPServer.cpp\
		src/ListeningSocket.cpp\
		src/MultiServer.cpp\
		src/SimpleServer.cpp\
		src/SimpleSocket.cpp\
		src/http_request/HTTPRequest.cpp\
		src/http_request/http_request_line/HTTPMethod.cpp\
		src/http_request/http_request_line/HTTPPath.cpp\
		src/http_request/http_request_line/HTTPVersion.cpp\
		src/http_request/http_request_line/RequestLineException.cpp\
		src/http_response/HTTPResponse.cpp\

OBJS = $(SRCS:.cpp=.o)

# COLORS #

RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
GREY = \033[2;37m
MAGENTA = \033[0;35m
CURSIVE = \033[3m
NO_COLOR = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
	@echo "\033[0;32mCompiled\033[0m"

echo: 
	echo $(OBJS)

clean:
	@rm $(NAME)
	@echo "\033[0;33mCleaning executable\033[0m"

fclean: clean
	@rm $(OBJS)
	@echo "\033[0;35mFclean done\033[0m"

re: fclean all
