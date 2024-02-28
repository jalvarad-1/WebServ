#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd;
    fd = open("/dev/stdin", O_RDONLY);
    if (fd == -1) {
        std::cout << "Status: 500 Internal server error" << std::endl;
        std::cout << "\r";
        std::cout << "Error: Cannot read input." << std::endl;
        return 1;
    }
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::cout << "Status: 500 Internal server error" << std::endl;
        std::cout << "\r\n";
        std::cout << "Error: Cannot read input." << std::endl;
        return 1;
    }
    pid_t pid = fork();
    if (pid == -1) {
        std::cout << "Status: 500 Internal server error" << std::endl;
        std::cout << "\r\n";
        std::cout << "Error: Cannot read input." << std::endl;
        return 1;
    } else if (pid == 0) {
        close(pipefd[0]);
        dup2(fd, STDIN_FILENO);
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(STDOUT_FILENO, STDERR_FILENO);
        close(pipefd[1]);
        char *python_args[] = {"python3", argv[1], NULL};
        execve("/usr/bin/python3", python_args, NULL);

        std::cout << "Status: 500 Internal server error" << std::endl;
        std::cout << "\r\n";
        std::cout << "Error: Cannot read input." << std::endl;
        return 1;
    } else {
        std::cout << "Status: 200 Ok" << std::endl;
        std::cout << "\r\n";
        close(pipefd[1]);
        char buffer[4096];
        ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer));
        while (bytesRead > 0) {
            std::cout.write(buffer, bytesRead);
            bytesRead = read(pipefd[0], buffer, sizeof(buffer));
        }
        close(pipefd[0]);
        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}