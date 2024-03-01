#ifndef CGI_HPP
# define CGI_HPP

# include "../../utils/utils.hpp"
# include "../routing/Response.hpp"
# include <map>
# include <sys/wait.h>
# include <fcntl.h>
# include <cstring>
# include <unistd.h>


class CGI
{
private:
    char **_envp;
    char **_argv;
    Response ret;
    char* _cgi_path;
    int set_error(int code, std::string body);
    int child_process(int (&pipefd)[2], int rd_fd);
    void parse_output(std::string output);

public:
    CGI(std::string cgi_path);
    ~CGI();
    void set_env(std::map<std::string, std::string> map);
    void set_args(std::vector<std::string> vec);
    int exec_cgi(std::string body_filename, pid_t *ret_pid);
};

#endif
