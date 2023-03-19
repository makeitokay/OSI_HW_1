#include "reverse.h"
#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

const int SIZE = 5000;

int main(int argc, char** argv) {
    char buffer[SIZE];

    char first_fifo_name[] = "5_1.fifo";
    char second_fifo_name[] = "5_2.fifo";

    createFifo(first_fifo_name);
    createFifo(second_fifo_name);

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

            int fd = getReadonlyFifo(second_fifo_name);

            readFromFd(fd, buffer, SIZE);

            char* output_filename = argv[2];
            writeToFile(output_filename, buffer);

            closeFd(fd);

            printf("Третий процесс: данные из канала считаны и записаны в файл.\n");
        } else {
            printf("Второй процесс: считываем данные из канала и проводим обработку данных.\n");

            int first_fd = getReadonlyFifo(first_fifo_name);

            readFromFd(first_fd, buffer, SIZE);
            reverse(buffer);

            int second_fd = getWriteonlyFifo(second_fifo_name);

            writeToFd(second_fd, buffer, SIZE);

            closeFd(first_fd);
            closeFd(second_fd);

            printf("Второй процесс: данные считаны, обработаны и записаны обратно в канал.\n");
        }

    } else {
        printf("Первый процесс: считываем входные данные из файла и отправляем в канал.\n");
        char* input_filename = argv[1];
        readFromFile(input_filename, buffer);

        int fd = getWriteonlyFifo(first_fifo_name);

        writeToFd(fd, buffer, SIZE);

        closeFd(fd);

        printf("Первый процесс: данные из входного файла считаны и записаны в канал.\n");
    }

    return 0;
}
