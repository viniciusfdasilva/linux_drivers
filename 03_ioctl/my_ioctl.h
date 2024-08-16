#ifndef MY_IOCTL_H
#define MY_IOCTL_H

#define int32_t unsigned int

struct s {
    int repeat;
    char name[64];
};

#define WR_VALUE _IOW('a', 'a', int32_t  *)
#define RD_VALUE _IOR('a', 'a', int32_t  *)
#define GREETER  _IOW('a', 'c', struct s *)
#endif