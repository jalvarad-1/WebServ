#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    char *python_args[] = {"bash", argv[0], NULL};
    std::cout << "Status: 200 Ok" << std::endl;
    std::cout << "Content-Type: text/plain" << std::endl;
    std::cout << "\r\n";
    execve("bash", python_args, NULL);
    return -1;
}