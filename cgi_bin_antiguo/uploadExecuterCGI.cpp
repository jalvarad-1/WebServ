#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
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
        char *python_args[] = {"bash", argv[1], argv[2], NULL};
        execve("/bin/bash", python_args, envp);
        std::cout << "Status: 500 Internal server error" << std::endl;
        std::cout << "\r\n";
        std::cout << "Error: Cannot read input." << std::endl;
        return 1;
    } else {
        // std::cout << "Status: 201 Ok" << std::endl;
        // std::cout << "Filename: 
        // std::cout << "\r\n";
        close(pipefd[1]);
        char buffer[4096];
        ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer));
        while (bytesRead > 0) {
            std::cout.write(buffer, bytesRead);
            bytesRead = read(pipefd[0], buffer, sizeof(buffer));
        }
        close(pipefd[0]);
        int status;
        waitpid(-1, &status, 0);
    }

    return 0;
}