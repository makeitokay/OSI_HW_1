#include "reverse.h"
#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

const int SIZE = 5000;

int main(int argc, char** argv) {
    char buffer[SIZE];

    char first_fifo_name[] = "8_1.fifo";
    char second_fifo_name[] = "8_2.fifo";

    createFifo(first_fifo_name);
    createFifo(second_fifo_name);

    printf("Первый процесс: считываем входные данные из файла и отправляем в первый канал.\n");
    char* input_filename = argv[1];
    readFromFile(input_filename, buffer);

    int first_fd = getWriteonlyFifo(first_fifo_name);

    writeToFd(first_fd, buffer, SIZE);

    printf("Первый процесс: данные из входного файла считаны и записаны в первый канал.\n");

    char second_buffer[SIZE];

    printf("Первый процесс: считываем данные из второго канала и записываем в файл.\n");

    int second_fd = getReadonlyFifo(second_fifo_name);

    readFromFd(second_fd, second_buffer, SIZE);

    char* output_filename = argv[2];
    writeToFile(output_filename, second_buffer);

    printf("Первый процесс: данные из второго канала считаны и записаны в файл.\n");

    closeFd(first_fd);
    closeFd(second_fd);

    return 0;
}
