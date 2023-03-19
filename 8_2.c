#include "reverse.h"
#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

const int SIZE = 5000;

int main() {
    char buffer[SIZE];

    char first_fifo_name[] = "8_1.fifo";
    char second_fifo_name[] = "8_2.fifo";

    createFifo(first_fifo_name);
    createFifo(second_fifo_name);

    printf("Второй процесс: считываем данные из первого канала и проводим обработку данных.\n");

    int first_fd = getReadonlyFifo(first_fifo_name);

    readFromFd(first_fd, buffer, SIZE);
    reverse(buffer);

    int second_fd = getWriteonlyFifo(second_fifo_name);

    writeToFd(second_fd, buffer, SIZE);

    closeFd(first_fd);
    closeFd(second_fd);

    printf("Второй процесс: данные считаны, обработаны и записаны во второй канал.\n");

    return 0;
}
