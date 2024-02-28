#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    // dup2(STDOUT_FILENO, STDERR_FILENO);
    char *python_args[] = {"python3", argv[0], NULL};
    std::cout << "Status: 200 Ok" << std::endl;
    std::cout << "Content-Type: text/plain" << std::endl;
    std::cout << "\r\n";
    execve("/usr/bin/python3", python_args, NULL);
    // std::cout << "Hola soy el body" << std::endl;
    // std::cout << "Status: 500 Internal server error" << std::endl;
    // std::cout << "\n\r";
    // std::cout << "Error: Cannot read input." << std::endl;
    return -1;
}