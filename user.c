#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "testdriver.h"

int main() {
    int answer, fd;
    struct mystruct test;

    fd = open("/dev/mydevice", O_WRONLY);
    if (fd < 0) {
        printf("Error opening character device /dev/mydevice\n");
        return 1;
    }

    ioctl(fd, RD_VALUE, &answer);
    printf("answer = %d\n", answer);

    answer = 123;
    ioctl(fd, WR_VALUE, &answer);

    ioctl(fd, RD_VALUE, &answer);
    printf("New answer = %d\n", answer);

    ioctl(fd, RD_STRUCT, &test);
    printf("test.num = %d, test.name = \"%s\"\n", test.num, test.name);

    test.num = 5;
    strncpy(test.name, "Chris", sizeof(test.name));
    ioctl(fd, WR_STRUCT, &test);

    ioctl(fd, RD_STRUCT, &test);
    printf("New test.num = %d, test.name = \"%s\"\n", test.num, test.name);

    close(fd);
    return 0;
}
