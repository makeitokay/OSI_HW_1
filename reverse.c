#include <sys/types.h>
#include <string.h>

void reverse(char str[]) {
    size_t len = strlen(str);
    for (int i = 0; i < len / 2; ++i) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}