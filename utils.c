//
// Created by makeitokay on 19.03.2023.

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

void readFromFile(char filename[], char result[]) {
    FILE* reader = fopen(filename, "r");
    if (reader == NULL) {
        printf("Не удалось открыть файл %s для чтения.\n", filename);
        exit(-1);
    }

    char ch;
    int size = 0;
    do {
        ch = fgetc(reader);
        result[size++] = ch;
    } while (ch != -1);
    result[size - 1] = '\0';

    fclose(reader);
}
void writeToFile(char filename[], char content[]) {
    FILE* writer = fopen(filename, "w");
    fputs(content, writer);
    fclose(writer);
}
void closeFd(int fd) {
    if (close(fd) < 0) {
        printf("Не удалось закрыть ресурс fd = %d.\n", fd);
        exit(-1);
    }
}
void readFromFd(int fd, char output[], int size) {
    int output_size = read(fd, output, size);
    if (output_size != size) {
        printf("Не удалось прочитать данные из fd = %d.\n", fd);
        exit(-1);
    }
}
void writeToFd(int fd, char content[], int size) {
    int result_size = write(fd, content, size);
    if (result_size != size) {
        printf("Не удалось записать данные в fd = %d.\n", fd);
        exit(-1);
    }
}

void createFifo(char name[]) {
    (void)umask(0);
    mkfifo(name, 0666);
}

int getReadonlyFifo(char name[]) {
    int fd = open(name, O_RDONLY);
    if (fd < 0) {
        printf("Не удалось открыть readonly fifo = %s\n", name);
    }
    return fd;
}

int getWriteonlyFifo(char name[]) {
    int fd = open(name, O_WRONLY);
    if (fd < 0) {
        printf("Не удалось открыть writeonly fifo = %s\n", name);
    }
    return fd;
}

void createPipe(int fd[]) {
    if (pipe(fd) < 0) {
        printf("Не удалось открыть неименованный канал.\n");
        exit(-1);
    }
}