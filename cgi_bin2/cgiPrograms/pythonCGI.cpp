#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    char *python_args[] = {"/usr/bin/python3", argv[1], NULL};
    std::cout << "Status: 200 Ok" << std::endl;
    std::cout << "Content-Type: text/plain" << std::endl;
    std::cout << "\r" << std::endl;
    execve("/usr/bin/python3", python_args, NULL);
    return -1;
}