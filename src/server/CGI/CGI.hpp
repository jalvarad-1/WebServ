#ifndef CGI_HPP
# define CGI_HPP

# include "../../utils/utils.hpp"
# include <map>
# include <sys/wait.h>
# include <fcntl.h>
# include <cstring>
# include <unistd.h>

//TODO review if it is necessary or wecan standarize the Response struct
struct CGI_Return
{
    std::map<std::string, std::string> headers;
    std::string body;
    int code;
};


class CGI
{
private:
    char **_envp;
    char **_argv;
    CGI_Return ret;
    char* _cgi_path;
    std::string _file_path;
    int execute_binary(std::string request_body);
    int set_error(int code, std::string body);
    int child_process(int (&pipefd)[2], std::string request_body);
    int father_process(int (&pipefd)[2], pid_t pid);
    void parse_output(std::string output);

public:
    CGI(std::string cgi_path, std::string file_path);
    ~CGI();
    CGI_Return run_CGI(std::string request_body);
    void set_env(std::map<std::string, std::string> map);
    void set_args(std::vector<std::string> vec);
};

#endif