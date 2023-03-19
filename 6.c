#include "reverse.h"
#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

const int SIZE = 5000;

int main(int argc, char** argv) {
    char buffer[SIZE];

    int first_fd[2];
    int second_fd[2];

    createPipe(first_fd);
    createPipe(second_fd);

    int chpid = fork();
    if (chpid == -1) {
        printf("Не удалось создать дочерний процесс от 1-го процесса.\n");
        exit(-1);
    } else if (chpid == 0) {
        printf("Второй процесс: считываем данные из первого канала и проводим обработку данных.\n");

        readFromFd(first_fd[0], buffer, SIZE);
        reverse(buffer);
        writeToFd(second_fd[1], buffer, SIZE);

        closeFd(first_fd[0]);
        closeFd(first_fd[1]);
        closeFd(second_fd[0]);
        closeFd(second_fd[1]);

        printf("Второй процесс: данные считаны, обработаны и записаны во второй канал.\n");

    } else {
        printf("Первый процесс: считываем входные данные из файла и отправляем в первый канал.\n");
        char* input_filename = argv[1];
        readFromFile(input_filename, buffer);

        writeToFd(first_fd[1], buffer, SIZE);

        printf("Первый процесс: данные из входного файла считаны и записаны в первый канал.\n");

        char second_buffer[SIZE];

        printf("Первый процесс: считываем данные из второго канала и записываем в файл.\n");
        readFromFd(second_fd[0], second_buffer, SIZE);

        char* output_filename = argv[2];
        writeToFile(output_filename, second_buffer);

        printf("Первый процесс: данные из второго канала считаны и записаны в файл.\n");

        closeFd(first_fd[0]);
        closeFd(first_fd[1]);
        closeFd(second_fd[0]);
        closeFd(second_fd[1]);
    }

    return 0;
}
