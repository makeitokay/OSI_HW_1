#include "reverse.h"
#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

const int SIZE = 5000;

int main(int argc, char** argv) {
    char buffer[SIZE];

    int fd[2];

    // открываем канал
    if (pipe(fd) < 0) {
        printf("Не удалось открыть неименованный канал.\n");
        exit(-1);
    }

    int chpid = fork();
    if (chpid == -1) {
        printf("Не удалось создать дочерний процесс от 1-го процесса.\n");
        exit(-1);
    } else if (chpid == 0) {
        chpid = fork();
        if (chpid == -1) {
            printf("Не удалось создать дочерний процесс от 2-го процесса.\n");
            exit(-1);
        } else if (chpid == 0) {
            printf("Третий процесс: считываем данные из канала и записываем в файл.\n");

            readFromFd(fd[0], buffer, SIZE);

            char* output_filename = argv[2];
            writeToFile(output_filename, buffer);

            closeFd(fd[0]);
            closeFd(fd[1]);

            printf("Третий процесс: данные из канала считаны и записаны в файл.\n");
        } else {
            printf("Второй процесс: считываем данные из канала и проводим обработку данных.\n");

            readFromFd(fd[0], buffer, SIZE);
            reverse(buffer);
            writeToFd(fd[1], buffer, SIZE);

            closeFd(fd[0]);
            closeFd(fd[1]);

            printf("Второй процесс: данные считаны, обработаны и записаны обратно в канал.\n");
        }

    } else {
        printf("Первый процесс: считываем входные данные из файла и отправляем в канал.\n");
        char* input_filename = argv[1];
        readFromFile(input_filename, buffer);

        writeToFd(fd[1], buffer, SIZE);

        closeFd(fd[0]);
        closeFd(fd[1]);

        printf("Первый процесс: данные из входного файла считаны и записаны в канал.\n");
    }

    return 0;
}
