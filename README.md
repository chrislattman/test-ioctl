# Communicating between user space and kernel space

Run `make` to build the kernel module and the user space code. Then run

```
sudo insmod testdriver.ko
```

to load the module. The module should register a character device (if anything goes wrong, run `sudo dmesg` to see what happened). Then run

```
cat /proc/devices | grep testdevice | head -n 1
```

This will return the device major number next to its name, which should be "testdevice" (Linux character devices have major and minor numbers). Use this major number by running

```
sudo mknod /dev/mydevice c <major-number> 0
```

This will make a filesystem node that exposes the character device to user space. Finally, run

```
sudo ./user
```

Now the user space code in user.c is communicating with the kernel space code in testdriver.c, and vice versa! When finished, run

```
sudo rmmod testdriver
sudo rm /dev/mydevice
```

This removes the kernel module, which unregisters the character device from earlier, and then removes the now-useless filesystem node from the filesystem.
