#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "my_ioctl.h"

#define assert(x, y) if(x == y) { perror("PANIC: Assertion failed!"); };

int main()
{
    int answer;

    struct s test = {4, "Vinicius"};
    int dev = open("/dev/dummy", O_WRONLY);
    
    assert(dev, -1);

    ioctl(dev, RD_VALUE, &answer);

    printf("The answer is %d\n", answer);

    answer = 123;
    ioctl(dev, WR_VALUE, &answer);
    ioctl(dev, RD_VALUE, &answer);
    printf("THE ANSWER IS NOW %d\n", answer);

    ioctl(dev, GREETER, &test);

    printf("Opening was sucessfull\n");
    close(dev);
    return 0;
}