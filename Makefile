CXX := g++
NAME := webserv
CXXFLAGS := -Wall -Wextra -std=c++98
SRCS =	src/main.cpp\
		src/server/HTTPServer.cpp\
		src/server/ListeningSocket.cpp\
		src/server/MultiServer.cpp\
		src/ConfigParser/Parser.cpp\
		src/ConfigParser/Tokenizer.cpp\
		src/ConfigParser/LocationRules.cpp\
		src/ConfigParser/ServerConfig.cpp\
		src/http_request/HTTPRequest.cpp\
        src/utils/utils.cpp\
		src/server/response_code/ResponseCode.cpp\
		src/server/routing/Routing_ns.cpp\


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
