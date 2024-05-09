#ifndef DRIVER_H
#define DRIVER_H

struct mystruct {
    int num;
    char name[64];
};

// this first and second arguments to these macros are unused
// these macros are defined in sys/ioctl.h for user space code
// and linux/ioctl.h for kernel modules
#define WR_VALUE  _IOW('a', 'a', int *)
#define RD_VALUE  _IOR('a', 'b', int *)
#define WR_STRUCT _IOW('a', 'c', struct mystruct *)
#define RD_STRUCT _IOR('a', 'd', struct mystruct *)

#endif // DRIVER_H
